#include "analyzer.hpp"

// helper methods
std::vector<Login> regex_match(const std::vector<std::string> log_lines);
std::string cut_prefix(const std::string& argument, const std::string& prefix);
std::vector<Device> filter_devices(std::vector<Login>& login_records);
std::vector<User> filter_users(std::vector<Login>& login_records);


void Analyzer::Initialize()
{
   std::cout << "Initializing Analyzer..." << std::endl;
}

void Analyzer::Start_Analysis(const std::vector<std::string> &logs)
{
   std::cout << "Starting Analysis..." << std::endl;

   std::vector<Login> login_records = regex_match(logs);
   std::vector<Device> devices = filter_devices(login_records);
   std::vector<User> users = filter_users(login_records);

   for(auto login : login_records)
         login.Print();

   for(auto device : devices)
         device.Print();
}

void Analyzer::Shutdown()
{
   std::cout << "Shutting down Analyzer..." << std::endl;
}



// helper methods
std::vector<Login> regex_match(const std::vector<std::string> log_lines)
{
   std::vector<Login> login_records;

   std::vector<std::string> macs;
   std::vector<std::string> logins;
   std::vector<std::string> dates;

   std::regex log_regex(R"regex(\"PDCSERVER\",\"IAS\",(\d\d\/\d\d\/\d\d\d\d),(\d\d:\d\d:\d\d),(\d*)?,(\"\w*(\\)?\w*\")?,\"(\w*\\\w*)\",\"([0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}):\w*\",\"([0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2}[-][0-9A-Fa-f]{2})\".*)regex");
   std::smatch m;

    for(auto line : log_lines)
    {
      if(!regex_match(line, m, log_regex))
         continue;

      // parts to extract from the line
      std::string mac;
      std::string login_name;
      std::string login_date;
      std::string login_time;

      /*
       * regex matching groups in the smatch
       * 1st group - date
       * 2nd group - time
       * 4th group - user name
       * 8th group - mac
       */
      auto i = 0;
      for (auto result : m)
      {
         //std::cout << "****" << result << std::endl;
         i++;
         if(i == 2)
            login_date = result;
         else if(i == 3)
            login_time = result;
         else if(i == 5)
            login_name = result;
         else if(i == 9)
            mac = result;
      }

      // adapt login_name, cut away quotes and prefix if domain is prefixed
      if(login_name != "")
      {
         login_name = login_name.substr(1, login_name.length() - 2);
         login_name = cut_prefix(login_name, "OTHR\\");
      }

      Login currentLogin(login_name, mac, login_date, login_time);
      login_records.push_back(currentLogin);
    }

    return login_records;
}

std::string cut_prefix(const std::string& argument, const std::string& prefix)
{
   if(argument.substr(0, prefix.size()) == prefix)
       return argument.substr(prefix.size());
   else
      return argument;
}

std::vector<Device> filter_devices(std::vector<Login>& login_records)
{
   std::vector<Device> devices;

   for(auto login : login_records)
   {
      std::string mac = login.getMac();
      std::string login_date = login.getLoginDate() + " - " + login.getLoginTime();

      // check if this device already exists and add login date in case
      bool isExisting(false);
      for(auto device : devices)
      {
         std::cout << "++++ mac: " << mac << ", device-mac: " << device.getMac() << std::endl;
         if(device.getMac() == mac)
         {
            std::cout << "Adding " << login_date << "to device-mac " << device.getMac() << std::endl;
            device.Add_Login_Date(login_date);
            isExisting = true;
         }
      }

      // if it doesn't exist, create new device
      if(!isExisting)
      {
         std::vector<std::string> login_dates;
         login_dates.push_back(login_date);
         Device currentDevice(mac, login_dates);
         devices.push_back(currentDevice);
      }
   }

   return devices;
}

std::vector<User> filter_users(std::vector<Login>& login_records)
{
   std::vector<User> users;
   std::vector<Device> devices;
   std::vector<std::string> user_names;

   for(auto login : login_records)
   {
      std::string login_name = login.getLoginName();
      // check if user name already exists
      if(!(std::find(user_names.begin(), user_names.end(), login_name) != user_names.end()))
      {
         // it does not exist yet
         user_names.push_back(login_name);

         // TODO:
         //User new_user();
         //users.push_back(new_user);
      }
   }

   return users;
}
