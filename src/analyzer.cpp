#include "analyzer.hpp"

// helper methods
void regex_match(const std::vector<std::string> log_lines, Database_Connection dbConn);
std::string cut_prefix(const std::string& argument, const std::string& prefix);
void filter_users(std::vector<Login>& login_records, Database_Connection dbConn);
void determineHandyMac(User& user, Database_Connection dbConn);
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
   Database_Connection con("mongodb://localhost:27017");
/*
   std::vector<std::string> collections;
   collections.push_back("hsp.logins");
   collections.push_back("hsp.users");
   con.DropCollections(collections);
*/
   // Init test data
   con.ReadFromFile(::test_file);
   std::vector<std::string> logs = con.GetLogContent();
   regex_match(logs, con);

   std::vector<Login> login_records = con.GetLogins();
   filter_users(login_records, con);

   std::vector<User> users = con.GetUsers();

   for(auto login : login_records)
      std::cout << login << std::endl << std::endl;

   for(auto& user : users) {
      determineHandyMac(user, con);
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
void regex_match(const std::vector<std::string> logLines, Database_Connection dbConn) {

   std::vector<Login> loginRecords;

   std::regex logRegex(R"regex(\"PDCSERVER\",\"IAS\",(\d\d\/\d\d\/\d\d\d\d),(\d\d:\d\d:\d\d),(\d*)?,(\"\w*(\\)?\w*\")?,\"(\w*\\\w*)\",\"([0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}):\w*\",\"([0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2})\".*)regex");
   std::smatch m;

    for(auto line : logLines) {

      if(!regex_match(line, m, logRegex))
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

std::string cut_prefix(const std::string& argument, const std::string& prefix) {

   if(argument.substr(0, prefix.size()) == prefix)
       return argument.substr(prefix.size());
   else
      return argument;
}

void filter_users(std::vector<Login>& login_records, Database_Connection dbConn) {
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

void determineHandyMac(User& user, Database_Connection dbConn) {
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
