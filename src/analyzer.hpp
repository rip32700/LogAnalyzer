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
private:
Database_Connection dbConn;
void regex_match(const std::vector<std::string> logLines) {

   std::vector<Login> loginRecords;

   std::regex logRegex(R"regex(\"PDCSERVER\",\"IAS\",(\d\d\/\d\d\/\d\d\d\d),(\d\d:\d\d:\d\d),(\d*)?,(\"\w*(\\)?\w*\")?,\"(\w*\\\w*)\",\"([0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}):\w*\",\"([0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2})\".*)regex");
   std::smatch m;

    for(auto line : logLines) {

      if(!std::regex_match(line, m, logRegex))
         continue;

      // parts to extract from the line
      std::string mac, loginName, loginDate, loginTime;

      /*
       * regex matching groups in the smatch
       * 1st group - date
       * 2nd group - time
       * 4th group - user name
       * 8th group - mac
       */
      auto i = 0;
      for (auto result : m) {
         //std::cout << "****" << result << std::endl;
         i++;
         if(i == 2)
            loginDate = result;
         else if(i == 3)
            loginTime = result;
         else if(i == 5)
            loginName = result;
         else if(i == 9)
            mac = result;
      }

      // adapt login_name, cut away quotes and prefix if domain is prefixed
      if(loginName != "")  {
         loginName = loginName.substr(1, loginName.length() - 2);
         loginName = cut_prefix(loginName, "OTHR\\");
      }

      // timestamp conversion
      std::string timeStampString(loginDate + " - " + loginTime);
      struct tm tm;
      strptime(timeStampString.c_str(), "%m/%d/%Y - %H:%M:%S", &tm);
      time_t timeStamp = mktime(&tm);

      Login currentLogin(loginName, mac, timeStamp);
      loginRecords.push_back(currentLogin);
    }

    // Write login data to collection and drop logLines
    dbConn.Upsert(loginRecords);
    std::vector<std::string> collections;
    collections.push_back("hsp.logs");
    dbConn.DropCollections(collections);
}

void filter_users(std::vector<Login>& login_records) {
   // Check every login for new User information.
   // New data will be added to the respective collections in MongoDB.

   for(auto& login : login_records) {
      std::string login_name(login.getLoginName());
      DeviceLogin currentDeviceLogin(login.getMac(), login.getTimeStamp());

      User user = dbConn.GetUserIfExistsOrInsert(login_name, currentDeviceLogin);
 
      // check if a login with same mac and timestamp already exists
      bool isLoginContained(false);
      for(auto& device_login : user.getLogins()) {
    	 if(device_login.getMac() == login.getMac() && device_login.getTimeStamp() == login.getTimeStamp()) {
            isLoginContained = true;
	    break;
	 }
      }

      if(!isLoginContained) {
	std::vector<DeviceLogin> logins = user.getLogins();
	logins.push_back(currentDeviceLogin);
	user.setLogins(logins);
	
	dbConn.Upsert(user);
      }
   }
}

std::string cut_prefix(const std::string& argument, const std::string& prefix) {

   if(argument.substr(0, prefix.size()) == prefix)
       return argument.substr(prefix.size());
   else
      return argument;
}

void determineHandyMac(User& user) {
   //std::cout << "MAC Frequencies for User " << user.getLoginName() << std::endl;

   std::vector<DeviceLogin> logins(user.getLogins());
   std::map<std::string, int> loginCorrelation;

   for(auto& login : logins)
      loginCorrelation[login.getMac()]++;

   int max(0);
   std::string handyMac;

   for(auto const &entry : loginCorrelation) {

      if(entry.second > max) {
         max = entry.second;
         handyMac = entry.first;
      }

      //std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
   }
   //std::cout << "Handy-Mac: " << handy_mac << std::endl;

   user.setHandyMac(handyMac);
   dbConn.Upsert(user);

   /* TODO: resolve mac address to vendor */
}

public:
   Analyzer(std::string uri) : dbConn(uri) {};
   ~Analyzer() {};
   void Initialize();
   void Start_Analysis();
   void Shutdown();
};

#endif
