#include "analyzer.hpp"

// test files
const std::string dhcp_log_file   = "files/DhcpSrvLog-Mon.log";
const std::string radius_log_file = "files/IN1602.log";
const std::string test_file       = "files/testfile.txt";
const std::string old_log_file    = "files/access.log";

const long standard_deviation = 100000; // in ms

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
   Regex_Match(logs);

   std::vector<Login> login_records = dbConn.GetLogins();
   Filter_Users(login_records);

   std::vector<User> users = dbConn.GetUsers();

   for(auto login : login_records)
      std::cout << login << std::endl << std::endl;

   for(auto& user : users) {
      Determine_Handy_Mac(user);
      std::cout << user << std::endl;
   }

   // check for anomalies
   for(auto& user : users)
      Check_Anomaly(user, ::standard_deviation);
}

void Analyzer::Shutdown()
{
   std::cout << "Shutting down Analyzer..." << std::endl;
}

void Analyzer::Check_Anomaly(const User& user, const long tolerance) 
{
   bool hasAnomaly(false);
   std::string handy_mac = user.getHandyMac();
   std::vector<time_t> handy_login_timestamps;
   std::vector<time_t> pc_login_timestamps;
   std::map<time_t, std::string> pc_login_mac;

   // Get timestamps of handy- and pc-logins
   for(auto login : user.getLogins()) {
      if(login.getMac() == handy_mac) {
         handy_login_timestamps.push_back(login.getTimeStamp());
      } else {
      	 pc_login_timestamps.push_back(login.getTimeStamp()); 
      	 pc_login_mac[login.getTimeStamp()] = login.getMac();
      }
   }

   // sort the timestamps
   std::sort(handy_login_timestamps.begin(), handy_login_timestamps.end());
   std::sort(pc_login_timestamps.begin(), pc_login_timestamps.end());

   time_t stdDeviation = Calculate_Standard_Deviation(pc_login_timestamps, handy_login_timestamps);
   
   for(auto pc_login_timestamp : pc_login_timestamps) {
   	   for(auto handy_login_timestamp : handy_login_timestamps) {
   	   	  tm* localPcLoginTime(localtime(&pc_login_timestamp));
   	   	  tm* localHandyLoginTime(localtime(&handy_login_timestamp));
   	   	  auto pcLoginDay = localPcLoginTime->tm_mday;
   	   	  auto handyLoginDay = localHandyLoginTime->tm_mday;

   	   	  if(pcLoginDay == handyLoginDay) {
   	   	  	  // compare difference from minutes
   	   	  	   auto diff = pc_login_timestamp - handy_login_timestamp;
                  if(diff < 0 || diff > stdDeviation) {
                     std::cout << "ANOMALY DETECTED FOR: " << user.getLoginName() << " on connecting with Device-MAC: " << pc_login_mac[pc_login_timestamp] 
                     << " at " << " - " << localPcLoginTime->tm_hour << ":" << localPcLoginTime->tm_min << ":"<< localPcLoginTime->tm_sec << std::endl;
                     hasAnomaly = true;                  
                  }
   	   	  }
   	   }
   }

   if(hasAnomaly) {
      /*
       * here, additional activities can be performed, when an anomaly was detected
       */
   }
}

time_t Analyzer::Calculate_Standard_Deviation(std::vector<time_t> pc_login_timestamps, std::vector<time_t> handy_login_timestamps) 
{
	time_t nearest(std::numeric_limits<time_t>::max());
   std::vector<time_t> allClosestDiffs;
   int count(0);

   for(auto pc_login_timestamp : pc_login_timestamps) {
	   count = 0;
	   nearest = std::numeric_limits<time_t>::max();
	   for(auto handy_login_timestamp : handy_login_timestamps) {
	   	  time_t diff = pc_login_timestamp - handy_login_timestamp;
	   	  if(diff < 0) {
	   	  	break;
	   	  }

	   	  nearest = diff;
	   	  count++;
	   }

	   if(count == 0) {
   		// no handy-login before first pc-login
   		continue;
	   }

   	allClosestDiffs.push_back(nearest);
  	   handy_login_timestamps.erase(handy_login_timestamps.begin(), handy_login_timestamps.begin()+count);
   }

	if (allClosestDiffs.size() == 0) {
		return 0;
	}

	time_t avg, sum(0);
	for(auto diff : allClosestDiffs) {
		sum += diff;
	}

	avg = sum / allClosestDiffs.size();

	time_t deviation(0);
	for(auto diff : allClosestDiffs) {
		deviation += (diff - avg) * (diff - avg);
	}

	time_t variation = deviation / allClosestDiffs.size();
	time_t stdDeviation = std::sqrt(variation);

	return stdDeviation;
}

/*
 * extracts login records from the given log lines via regex machting
 * and stores them into the database
 */
void Analyzer::Regex_Match(const std::vector<std::string> logLines) 
{
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
         loginName = Cut_Prefix(loginName, "OTHR\\");
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

/*
 * extracts user objects form given login objects
 */
void Analyzer::Filter_Users(std::vector<Login>& login_records) 
{
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

std::string Analyzer::Cut_Prefix(const std::string& argument, const std::string& prefix) 
{
   if(argument.substr(0, prefix.size()) == prefix)
       return argument.substr(prefix.size());
   else
      return argument;
}

void Analyzer::Determine_Handy_Mac(User& user) 
{
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
   }

   user.setHandyMac(handyMac);
   dbConn.Upsert(user);

   /*
    * the mac address could further be resolved to a vendor, when an appropriate 
    * database to perform the lookup is available 
    */
}
