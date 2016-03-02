/*
 * Device.hpp
 *
 *  Created on: Feb 27, 2016
 *      Author: phoebus
 */

#ifndef DEVICE_HPP_
#define DEVICE_HPP_

#include <string>
#include <vector>
#include <iostream>

/**
  * @author: prieger
  * Device object
  * - represents a device 
  * - contains the device's mac address and the login timestamps
  *
  * OUTDATED CLASS - remains in project due to database setup
  */

class Device
{
private:
	std::string mac;
	std::vector<std::string> login_dates;

public:
	Device(const std::string& mac, const std::vector<std::string>& login_dates)
	{
		this->mac = mac;
		this->login_dates = login_dates;
	}
	~Device() {}

	void Add_Login_Date(const std::string& login_date)
	{
		std::cout << "Adding date (" << login_date << ") to mac " << mac << std::endl;
		login_dates.push_back(login_date);
		std::cout << "All current dates for mac " << mac << std::endl;
		for(auto login_date : login_dates)
		{
			std::cout << "**** " << login_date << std::endl;
		}
	}

	void Print()
	{
		std::cout << "Device Details: " << std::endl;
		std::cout << "---> MAC: " << mac << std::endl;
		for(auto login : login_dates)
		{
			std::cout << "---> Login on : " << login << std::endl;
		}
	}

	const std::vector<std::string>& getLoginDates() const {
		return login_dates;
	}

	void setLoginDates(const std::vector<std::string>& loginDates) {
		login_dates = loginDates;
	}

	const std::string& getMac() const {
		return mac;
	}

	void setMac(const std::string& mac) {
		this->mac = mac;
	}
};

#endif /* DEVICE_HPP_ */
