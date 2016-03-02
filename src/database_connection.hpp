#ifndef H_DATABASE_CONNECTION
#define H_DATABASE_CONNECTION
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include "mongo/client/dbclient.h"

#include <iostream>
#include <fstream>
#include <ctime>

#include "login.hpp"
#include "user.hpp"
#include "device.hpp"
#include "device_login.hpp"
#include "boost/lexical_cast.hpp"

/**
  * @author: prieger, mdorr
  * Database Connection object
  * - represents a connection to the mongo db
  * - provides a connect/drop methods and various
  *   functions for database operations
  *
  * - implementations are found in the corresponding CPP-file
  */

class Database_Connection
{
public:
   
   /* CONSTRUCTOR and DESTRUCTOR */

   Database_Connection(std::string uri) {
	     this->dbClient = connect(uri);
   };

   ~Database_Connection() {};

   /* FUNCTIONS */

   std::vector<std::string> GetLogContent();
   std::vector<Login> GetLogins();
   std::vector<User> GetUsers();
   User GetUserIfExistsOrInsert(std::string loginName, DeviceLogin currentLogin);

   void DropCollections(std::vector<std::string> collectionNames);
   void ReadFromFile(std::string fileName);
   void Upsert(Login value);
   void Upsert(Device value);
   void Upsert(User value);
   void Upsert(std::vector<Login> values);
   void Upsert(std::vector<Device> values);
   void Upsert(std::vector<User> values);

private:

   /* MEMBER ATTRIBUTES */

   mongo::DBClientBase* dbClient;

   /* PRIVATE FUNCTIONS */

   inline mongo::DBClientBase* connect(std::string uri) {
      mongo::client::GlobalInstance instance;
      std::string errmsg;
      mongo::ConnectionString cs = mongo::ConnectionString::parse(uri, errmsg);
      return cs.connect(errmsg);
   };

   inline std::vector<mongo::BSONObj> getAll(std::string collectionName) {
      auto cursor = dbClient->query(collectionName, mongo::BSONObj());
      if (!cursor.get()) {
          std::cout << "query failure" << std::endl;
      }

      std::vector<mongo::BSONObj> retVector;
      while (cursor->more()) {
          retVector.push_back(cursor->next().getOwned());
      }

      return retVector;
   }
};

#endif
