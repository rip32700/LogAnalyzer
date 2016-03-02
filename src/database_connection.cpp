#include "database_connection.hpp"
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

using namespace std;
using namespace mongo;

mongo::DBClientBase* Database_Connection::Connect(std::string uri)
{
	mongo::client::GlobalInstance instance;
	std::string errmsg;

	ConnectionString cs = ConnectionString::parse(uri, errmsg);
	return cs.connect(errmsg);
}

std::vector<std::string> Database_Connection::GetAll(mongo::DBClientBase* conn, std::string collectionName)
{
	auto cursor = conn->query(collectionName, BSONObj());
	if (!cursor.get()) {
	    cout << "query failure" << endl;
	}

	std::vector<std::string> retVector;

	while (cursor->more()) {
	    retVector.push_back(cursor->next().getStringField("log"));
	}

	return retVector;
}

void Database_Connection::ReadFromFile(mongo::DBClientBase* conn, std::string fileName)
{
	std::ifstream infile(fileName);
	std::string line;

	while (std::getline(infile, line))
	{
	    BSONObj p = BSON("log" << line);
	    conn->insert("hsp.logs", p);
	}
}

void Database_Connection::DropCollection(mongo::DBClientBase* conn, std::string collectionName)
{
	cout << "Dropping old Collection" << endl;
	conn->dropCollection(collectionName);
}

void Database_Connection::InsertBSONObj(mongo::DBClientBase* conn, std::string collectionName, mongo::BSONObj obj)
{
	conn->insert(collectionName, obj);
}

void Database_Connection::InsertBSONObj(mongo::DBClientBase* conn, std::string collectionName, std::vector<mongo::BSONObj>& obj)
{
	conn->insert(collectionName, obj);
}

BSONObj Database_Connection::CreateBSON(Login data)
{
	return BSON("loginname" << data.getLoginName() << "mac" << data.getMac() << "logindate" << boost::lexical_cast<std::string>(data.getTimeStamp()));
}

BSONObj Database_Connection::CreateBSON(User data)
{

}

BSONObj Database_Connection::CreateBSON(Device data)
{

}
