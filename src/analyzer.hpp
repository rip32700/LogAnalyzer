#ifndef H_ANALYZER
#define H_ANALYZER

#include <iostream>

class Analyzer
{
public:
   Analyzer() {};
   ~Analyzer() {};
   void Initialize();
   void Start_Analysis(const std::string &logs);
   void Shutdown();
};

#endif
