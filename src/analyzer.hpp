#ifndef H_ANALYZER
#define H_ANALYZER

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <algorithm>

#include "login.hpp"
#include "device_login.hpp"
#include "user.hpp"

class Analyzer
{
public:
   Analyzer() {};
   ~Analyzer() {};
   void Initialize();
   void Start_Analysis(const std::vector<std::string> &logs);
   void Shutdown();
};

#endif
