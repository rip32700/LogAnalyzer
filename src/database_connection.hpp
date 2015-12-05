#ifndef H_DATABASE_CONNECTION
#define H_DATABASE_CONNECTION

#include <iostream>

class Database_Connection
{
private:

public:
   Database_Connection() {};
   ~Database_Connection() {};
   void Connect();
   void Disconnect();
   void ExecuteQuery();
   void FindById();
   void Update();
   void Delete();
};

#endif
