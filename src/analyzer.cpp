#include "analyzer.hpp"

void Analyzer::Initialize()
{
   std::cout << "Initializing Analyzer..." << std::endl;
}

void Analyzer::Start_Analysis(const std::vector<std::string> &lines)
{
   std::cout << "Starting Analysis..." << std::endl;

   std::vector<std::string> macs;
   std::regex log_regex(R"regex(.*DHCP:.*REQUEST\sfrom\s(\w*:\w*:\w*:\w*:\w*:\w*).*)regex");
   std::smatch m;

   // gather MAC addresses
   for(auto line : lines) 
   {
   		regex_match(line, m, log_regex);

		auto i = 0;
		for (auto result : m)
		{
			// matched group (mac-addr) is in the second element of the smatch
			if(i++ == 1)
			{
				// if not in the vector already - add it
				if (std::find(macs.begin(), macs.end(), result) == macs.end()) {
					macs.push_back(result);
				}
			}
		}
   }

   // output the result
   std::cout << "Found following MACs:" << std::endl;
   for(auto mac : macs)
   {
	   std::cout << "[+] " << mac << std::endl;
   }

}

void Analyzer::Shutdown()
{
   std::cout << "Shutting down Analyzer..." << std::endl;
}