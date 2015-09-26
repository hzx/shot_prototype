#ifndef SHOT_DBCLIENT_H
#define SHOT_DBCLIENT_H


#include <string>
#include <vector>
#include <mongo/bson/bson.h>
#include <mongo/client/dbclient.h>


using std::string;
using std::vector;


namespace shot {


std::string newId();


class Query {
public:
  Query(string table);
  Query& find();
  Query& findOne();
  Query& limit();
private:
  string table;
};


class DbClient {
public:
  DbClient();
  static DbClient& instance();
  bool init(string host, string port, string user, string password, string name);

  // TODO: implement CRUD, all include DbQuery query param
  bool insert(Query& query);
  bool find(Query& query);
  bool findOne(Query& query);
  bool update(Query& query);
  bool remove(Query& query);

  mongo::DBClientConnection conn;
  string host;
  string port;
  string user;
  string password;
  string name;
private:
};


} /* namespace shot */

#endif /* end of include guard: SHOT_DBCLIENT_H */
