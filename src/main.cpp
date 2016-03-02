#include <iostream>
#include <stdlib.h>

#include "login.hpp"
#include "user.hpp"
#include "file_reader.hpp"

#include "mongo/client/dbclient.h"
#include "analyzer.cpp"

/**
  * @author: prieger
  * main function of the program
  * - connects to mongo db
  * - starts the analyzer functionality
 */

int main(int argc, char **argv) 
{
   Analyzer analyzer("mongodb://localhost:27017");
   File_Reader f_reader;

   // INIT
   analyzer.Initialize();
   analyzer.Start_Analysis();

   // SHUTDOWN
   analyzer.Shutdown();

   return EXIT_SUCCESS;
}
