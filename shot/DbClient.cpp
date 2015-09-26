#include <iostream>
#include "DbClient.h"


using std::string;


namespace shot {


std::string newId() {
  return mongo::OID::gen().toString();
}


Query::Query(string table) {
  this->table = table;
}


Query& Query::find() {
  return *this;
}


Query& Query::findOne() {
  return *this;
}


Query& Query::limit() {
  return *this;
}


DbClient::DbClient()
    : conn(true) {
}


DbClient& DbClient::instance() {
  static DbClient client;
  return client;
}


bool DbClient::init(string host, string port, string user, string password, string name) {
  this->host = host;
  this->port = port;
  this->user = user;
  this->password = password;
  this->name = name;

  mongo::Status status = mongo::client::initialize();
  if (!status.isOK()) {
    std::cout << "[db]: failed to initialize the client driver" << status << std::endl;
    return false;
  }

  string errmsg;
  if (!conn.connect(host + ':' + port, errmsg)) {
    std::cout << "[db]: failed connect: " << errmsg << std::endl;
    return false;
  }

  return true;
}


bool DbClient::insert(Query& query) {
  // insert test records
  /* BSONObjBuilder obj; */
  /* obj.append("name", "Квартира 1 комнатная"); */
  /* obj.append("price", 2000000); */
  string table = name + ".record";
  conn.insert(table,
    BSON( "name" << "Квартира 2 комнатная" << "price" << 2500000 ));
  string e = conn.getLastError();
  if (!e.empty()) {
    std::cout << "insert failed: " << e << std::endl;
    return false;
  }
  return true;
}


bool DbClient::find(Query& query) {
  return true;
}


bool DbClient::findOne(Query& query) {
  return true;
}


bool DbClient::update(Query& query) {
  return true;
}


bool DbClient::remove(Query& query) {
  return true;
}


} /* namespace shot */
