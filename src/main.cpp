#include <iostream>
#include <stdlib.h>

#include "login.hpp"
#include "device.hpp"
#include "user.hpp"

#include "mongo/client/dbclient.h"
#include "analyzer.cpp"
#include "database_connection.cpp"
#include "file_reader.cpp"

const std::string dhcp_log_file   = "files/DhcpSrvLog-Mon.log";
const std::string radius_log_file = "files/IN1602.log";
const std::string test_file       = "files/testfile.txt";
const std::string old_log_file    = "files/access.log";

int main(int argc, char **argv) 
{
   Analyzer analyzer;
   Database_Connection con;
   File_Reader f_reader;

   // INIT

   mongo::DBClientBase* dbClient = con.Connect("mongodb://localhost:27017");
   con.DropCollection(dbClient, "hsp.logs");
   con.ReadFromFile(dbClient, ::test_file);
   std::vector<std::string> logs = con.GetAll(dbClient, "hsp.logs");

   analyzer.Initialize();
   analyzer.Start_Analysis(logs);

   // SHUTDOWN
   analyzer.Shutdown();

   return EXIT_SUCCESS;
}
