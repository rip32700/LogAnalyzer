#ifndef H_DATABASE_CONNECTION
#define H_DATABASE_CONNECTION

#include "mongo/client/dbclient.h"
#include <iostream>

using namespace mongo;

class Database_Connection
{
private:

public:
   Database_Connection() {};
   ~Database_Connection() {};
   mongo::DBClientBase* Connect(std::string& uri);
   mongo::DBClientCursor* GetAll(mongo::DBClientBase* conn, std::string tableName);
   void InsertBSONObj(mongo::DBClientBase* conn, std::string tableName, mongo::BSONObj obj);
   void InsertBSONObj(mongo::DBClientBase* conn, std::string tableName, std::vector<mongo::BSONObj>& obj);
};

#endif
