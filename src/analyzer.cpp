#include "analyzer.hpp"

// helper methods
void check_anomaly(const User& user, const long tolerance);
time_t calculateStandardDeviation(std::vector<time_t> pc_login_timestamps, std::vector<time_t> handy_login_timestamps);

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

   time_t stdDeviation = calculateStandardDeviation(pc_login_timestamps, handy_login_timestamps);
   
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

   }
}

time_t calculateStandardDeviation(std::vector<time_t> pc_login_timestamps, std::vector<time_t> handy_login_timestamps) {
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
