#include <iostream>
#include <stdlib.h>

#include "analyzer.cpp"
#include "database_connection.cpp"
#include "file_reader.cpp"

const std::string file_name = "../files/testfile.txt";

int main(int argc, char **argv) 
{
   Analyzer analyzer;
   Database_Connection con;
   File_Reader f_reader;

   con.Connect();
   analyzer.Initialize();
   
   // analysis start, provide logs either by
   // database or by file reader
   std::cout << std::endl;
   std::string logs = f_reader.Read_File(::file_name);
   analyzer.Start_Analysis(logs);
   std::cout << std::endl << std::endl;

   analyzer.Shutdown();
   con.Disconnect();

   return EXIT_SUCCESS;
}
