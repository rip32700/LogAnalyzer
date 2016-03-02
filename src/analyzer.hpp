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
#include <limits>

#include "login.hpp"
#include "device_login.hpp"
#include "user.hpp"
#include "database_connection.cpp"

/** 
  * @author: prieger, akalofogados
  * Actual Analyzer object
  * - provides the main functionality of the program
  * - 
  */

class Analyzer
{
public:

	/* CONSTRUCTOR and DESTRUCTOR */
    Analyzer(std::string uri) : dbConn(uri) {};
   
    ~Analyzer() {};

    /* FUNCTIONS */

    void Initialize();
    void Start_Analysis();
    void Shutdown();

private:

	/* MEMBER ATTRIBUTES */

	Database_Connection dbConn;

	/* PRIVATE FUNCTIONS */

	void Check_Anomaly(const User& user, const long tolerance);
	time_t Calculate_Standard_Deviation(std::vector<time_t> pc_login_timestamps, std::vector<time_t> handy_login_timestamps);
	void Regex_Match(const std::vector<std::string> logLines);
	void Filter_Users(std::vector<Login>& login_records);
	std::string Cut_Prefix(const std::string& argument, const std::string& prefix);
	void Determine_Handy_Mac(User& user);
};

#endif
