#include "analyzer.hpp"

// helper methods
std::vector<Login> regex_match(const std::vector<std::string> log_lines);
std::string cut_prefix(const std::string& argument, const std::string& prefix);
std::vector<User> filter_users(std::vector<Login>& login_records);
void determineHandyMac(User& user);
void check_anomaly(const User& user, const long tolerance);

const long standard_deviation = 5; // in minutes

void Analyzer::Initialize()
{
   std::cout << "Initializing Analyzer..." << std::endl;
}

void Analyzer::Start_Analysis(const std::vector<std::string> &logs)
{
   std::cout << "Starting Analysis..." << std::endl;

   std::vector<Login> login_records(regex_match(logs));
   std::vector<User> users(filter_users(login_records));


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
std::vector<Login> regex_match(const std::vector<std::string> logLines) {

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

    return loginRecords;
}

std::string cut_prefix(const std::string& argument, const std::string& prefix) {

   if(argument.substr(0, prefix.size()) == prefix)
       return argument.substr(prefix.size());
   else
      return argument;
}

std::vector<User> filter_users(std::vector<Login>& login_records) {

   std::vector<User> users;
   std::vector<Device> devices;
   std::vector<std::string> user_names;

   for(auto& login : login_records) {
      std::string login_name(login.getLoginName());
      DeviceLogin currentDeviceLogin(login.getMac(), login.getTimeStamp());

      // check if user name already exists
      if(!(std::find(user_names.begin(), user_names.end(), login_name) != user_names.end())) {
         // it does not exist yet, so put in list and create new user
         user_names.push_back(login_name);
         std::vector<DeviceLogin> logins;
         logins.push_back(currentDeviceLogin);
         User newUser(login_name, logins);
         users.push_back(newUser);

      } else {
         // user already exists, so insert
         for(auto& user : users) {
            if(user.getLoginName() == login_name) {
               // check whether first if a login with same mac and login already exists
               bool isLoginContained(false);
               for(auto& device_login : user.getLogins()) {
                  if(device_login.getMac() == login.getMac() && device_login.getTimeStamp() == login.getTimeStamp())
                     isLoginContained = true;
               }
               if(!isLoginContained)
                  user.Add_Device_Login(currentDeviceLogin);
               break;
            }
         }

      }
   }

   return users;
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