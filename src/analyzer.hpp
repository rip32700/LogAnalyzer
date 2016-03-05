#ifndef H_ANALYZER
#define H_ANALYZER

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <regex>
#include <algorithm>
#include <ctime>
#include <cmath>

#include "login.hpp"
#include "device_login.hpp"
#include "user.hpp"
#include "database_connection.cpp"

class Analyzer
{
public:
   Analyzer() {};
   ~Analyzer() {};
   void Initialize();
   void Start_Analysis();
   void Shutdown();
};

#endif
