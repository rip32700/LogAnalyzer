#include "database_connection.hpp"
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include "mongo/client/dbclient.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace mongo;

int run(DBClientBase* conn) {
    cout << "connected ok" << endl;
    std::ifstream infile("../files/access.log");

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        string month, day, time, ip, dhcp, actor, action, type, direction, mac;
	iss >> month >> day >> time >> ip >> dhcp >> actor >> action >> type >> direction >> mac;
	if (type == "REQUEST") {

            BSONObj log = BSON("Month" << month
                             	<< "Day" << day
				<< "Time" << time
				<< "Ip Adress" << ip
				<< "Mac Adress" << mac);
            conn->insert("test.access", log);
	}
    }

    cout << "count:" << conn->count("test.access") << endl;

    std::auto_ptr<DBClientCursor> cursor = conn->query("test.access", BSONObj());
    if (!cursor.get()) {
        cout << "query failure" << endl;
        return EXIT_FAILURE;
    }

    while (cursor->more()) {
        cout << cursor->next().toString() << endl;
    }

    return EXIT_SUCCESS;
}

/*
int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "usage: " << argv[0] << " [MONGODB_URI]" << std::endl;
        return EXIT_FAILURE;
    }

    mongo::client::GlobalInstance instance;
    if (!instance.initialized()) {
        std::cout << "failed to initialize the client driver: " << instance.status() << std::endl;
        return EXIT_FAILURE;
    }

    std::string uri = argc == 2 ? argv[1] : "mongodb://localhost:27017";
    std::string errmsg;

    ConnectionString cs = ConnectionString::parse(uri, errmsg);

    if (!cs.isValid()) {
        std::cout << "Error parsing connection string " << uri << ": " << errmsg << std::endl;
        return EXIT_FAILURE;
    }

    boost::scoped_ptr<DBClientBase> conn(cs.connect(errmsg));
    if (!conn) {
        cout << "couldn't connect : " << errmsg << endl;
        return EXIT_FAILURE;
    }

    int ret = EXIT_SUCCESS;
    try {
        ret = run(conn.get());
    } catch (DBException& e) {
        cout << "caught " << e.what() << endl;
        ret = EXIT_FAILURE;
    }
    return ret;
}
*/


void Database_Connection::Connect()
{
	std::cout << "Establishing Database Connection..." << std::endl;
}

void Database_Connection::Disconnect()
{
	std::cout << "Shutting down Database Connection..." << std::endl;
}

void Database_Connection::ExecuteQuery()
{

}

void Database_Connection::FindById()
{

}

void Database_Connection::Update()
{

}

void Database_Connection::Delete()
{

}
