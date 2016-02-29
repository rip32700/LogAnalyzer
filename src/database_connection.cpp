#include "database_connection.hpp"
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include "mongo/client/dbclient.h"
#include <iostream>

using namespace std;
using namespace mongo;


mongo::DBClientBase* Database_Connection::Connect(std::string& uri)
{
	mongo::client::GlobalInstance instance;
	std::string errmsg;

	ConnectionString cs = ConnectionString::parse(uri, errmsg);
	boost::scoped_ptr<DBClientBase> conn(cs.connect(errmsg));
	
	return conn.get();
}

mongo::DBClientCursor* Database_Connection::GetAll(mongo::DBClientBase* conn, std::string tableName)
{
	auto cursor = conn->query(tableName, BSONObj());
	if (!cursor.get()) {
	    cout << "query failure" << endl;
	}

	return cursor.get();

	/*
	while (cursor->more()) {
	    cout << cursor->next().toString() << endl;
	}
	*/
}

void Database_Connection::InsertBSONObj(mongo::DBClientBase* conn, std::string tableName, mongo::BSONObj obj)
{
	conn->insert(tableName, obj);
}

void Database_Connection::InsertBSONObj(mongo::DBClientBase* conn, std::string tableName, std::vector<mongo::BSONObj>& obj)
{
	conn->insert(tableName, obj);
}
