#include "analyzer.hpp"

// helper methods
void check_anomaly(const User& user, const long tolerance);

// test files
const std::string dhcp_log_file   = "files/DhcpSrvLog-Mon.log";
const std::string radius_log_file = "files/IN1602.log";
const std::string test_file       = "files/testfile.txt";
const std::string old_log_file    = "files/access.log";

const long standard_deviation = 5; // in minutes

void Analyzer::Initialize()
{
   std::cout << "Initializing Analyzer..." << std::endl;
}

void Analyzer::Start_Analysis()
{
   std::cout << "Starting Analysis..." << std::endl;
/*
   std::vector<std::string> collections;
   collections.push_back("hsp.logins");
   collections.push_back("hsp.users");
   dbConn.DropCollections(collections);
*/
   // Init test data
   dbConn.ReadFromFile(::test_file);
   std::vector<std::string> logs = dbConn.GetLogContent();
   regex_match(logs);

   std::vector<Login> login_records = dbConn.GetLogins();
   filter_users(login_records);

   std::vector<User> users = dbConn.GetUsers();

   for(auto login : login_records)
      std::cout << login << std::endl << std::endl;

   for(auto& user : users) {
      determineHandyMac(user);
      std::cout << user << std::endl;
   }

   // check for anomalies
   for(auto& user : users)
      check_anomaly(user, ::standard_deviation);
}

void Analyzer::Shutdown()
{
   std::cout << "Shutting down Analyzer..." << std::endl;
}



// helper methods
void check_anomaly(const User& user, const long tolerance) {
   bool hasAnomaly(false);
   std::string handy_mac = user.getHandyMac();
   std::vector<time_t> handy_login_timestampes;

   for(auto login : user.getLogins())
      if(login.getMac() == handy_mac)
         handy_login_timestampes.push_back(login.getTimeStamp());

   std::map<std::string, std::vector<std::string>> correlations;
   for(auto handy_timestamp : handy_login_timestampes)   {
      for(auto login : user.getLogins()) {
         if(login.getMac() != handy_mac) {

            // check if they are on the same day
            time_t loginTime(login.getTimeStamp());
            tm* locaLoginTimeStamp(localtime(&loginTime));
            auto loginDay = locaLoginTimeStamp->tm_mday;

            tm*  localHandyLoginTime(localtime(&handy_timestamp));
            auto handyLoginDay = localHandyLoginTime->tm_mday;

            if(loginDay == handyLoginDay) {

               // compare difference from minutes
               if(std::abs(loginTime - handy_timestamp) > tolerance) {
                  std::cout << "ANOMALY DETECTED FOR: " << user.getLoginName() << std::endl;
                  hasAnomaly = true;
               }

            }
         }
      }
   }

   if(hasAnomaly) {

   }
}
