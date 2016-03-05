#include <iostream>
#include <stdlib.h>

#include "login.hpp"
#include "device.hpp"
#include "user.hpp"

#include "mongo/client/dbclient.h"
#include "analyzer.cpp"
#include "file_reader.cpp"

int main(int argc, char **argv) 
{
   Analyzer analyzer;
   File_Reader f_reader;

   // INIT
   analyzer.Initialize();
   analyzer.Start_Analysis();

   // SHUTDOWN
   analyzer.Shutdown();

   return EXIT_SUCCESS;
}
