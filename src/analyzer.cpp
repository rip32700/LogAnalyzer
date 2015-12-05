#include "analyzer.hpp"

void Analyzer::Initialize()
{
   std::cout << "Initializing Analyzer..." << std::endl;
}

void Analyzer::Start_Analysis(const std::string &logs)
{
   std::cout << "Starting Analysis..." << std::endl;
   std::cout << "Got as input:" << std::endl;
   std::cout << logs;
}

void Analyzer::Shutdown()
{
   std::cout << "Shutting down Analyzer..." << std::endl;
}