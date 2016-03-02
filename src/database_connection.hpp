#ifndef H_DATABASE_CONNECTION
#define H_DATABASE_CONNECTION

#include "mongo/client/dbclient.h"
#include <iostream>
#include <fstream>

#include "login.hpp"
#include "user.hpp"
#include "device.hpp"
#include "boost/lexical_cast.hpp"

using namespace mongo;

class Database_Connection
{
private:

public:
   Database_Connection() {};
   ~Database_Connection() {};
   mongo::DBClientBase* Connect(std::string uri);
   std::vector<std::string> GetAll(mongo::DBClientBase* conn, std::string collectionName);
   void DropCollection(mongo::DBClientBase* conn, std::string collectionName);
   void ReadFromFile(mongo::DBClientBase* conn, std::string fileName);
   void InsertBSONObj(mongo::DBClientBase* conn, std::string collectionName, mongo::BSONObj obj);
   void InsertBSONObj(mongo::DBClientBase* conn, std::string collectionName, std::vector<mongo::BSONObj>& obj);
   BSONObj CreateBSON(Login data);
   BSONObj CreateBSON(User data);
   BSONObj CreateBSON(Device data);
};

#endif
