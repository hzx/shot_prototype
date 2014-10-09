#include "shot/Config.h"
#include <boost/program_options.hpp>
#include "shot/utils.h"


namespace po = boost::program_options;


namespace shot {


Config::Config()
    : port(DEFAULT_PORT)
    , maxEvents(DEFAULT_MAX_EVENTS)
    , chunkSize(DEFAULT_CHUNK_SIZE)
    , filename(DEFAULT_CONFIG_FILENAME)
    , tmp("/tmp/")
    , dbhost("127.0.0.1")
    , dbport("27017")
    , dbuser("")
    , dbpassword("")
    , dbname("test") {
  directory = getDirectory();
}


Config::~Config() {
}


Config& Config::instance() {
  static Config config;
  return config;
}


int Config::parseCmd(int argc, const char* argv[]) {
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("port", po::value<int>(), "listen port")
      ("host", po::value<std::string>(), "site domain")
      /* ("config", po::value<>(), "config filename") */
      ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("port")) {
      port = vm["port"].as<int>();
    }
    if (vm.count("host")) {
      host = vm["host"].as<std::string>();
    }
  } catch (std::exception& e) {
    return -1;
  } catch (...) {
    return -2;
  }

  return 0;
}


int Config::parseFile(const char* filename) {
  return 0;
}


} /* namespace shot */
