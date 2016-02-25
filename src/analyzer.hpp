#ifndef H_ANALYZER
#define H_ANALYZER

#include <iostream>
#include <fstream>
#include <regex>
#include <string>

class Analyzer
{
public:
   Analyzer() {};
   ~Analyzer() {};
   void Initialize();
   void Start_Analysis(const std::vector<std::string> &lines);
   void Shutdown();
};

#endif
