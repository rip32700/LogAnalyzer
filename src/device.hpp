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
		login_dates.push_back(login_date);
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
