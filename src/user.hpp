/*
 * User.hpp
 *
 *  Created on: Feb 27, 2016
 *      Author: phoebus
 */

#ifndef USER_HPP_
#define USER_HPP_

#include <string>
#include <vector>
#include <iostream>
#include "device_login.hpp"

class User
{
private:
	std::string login_name;
	std::vector<DeviceLogin> logins;

public:
	User(const std::string& login_name, const std::vector<DeviceLogin>& logins)
	{
		this->login_name = login_name;
		this->logins = logins;
	}
	~User() { }

	void Add_Device_Login(const DeviceLogin& device_login)
	{
		this->logins.push_back(device_login);
	}

	void Print()
	{
		std::cout << "User Details: " << std::endl;
		std::cout << "-> Login Name: " << login_name << std::endl;
		std::cout << "-> Logins: " << std::endl;
		for(auto login : logins)
		{
			//std::cout << "---> *** ";
			login.Print();
			//std::cout << std::endl;
		}
	}

	const std::string& getLoginName() const {
		return login_name;
	}

	void setLoginName(const std::string& loginName) {
		login_name = loginName;
	}

	const std::vector<DeviceLogin>& getLogins() const {
		return logins;
	}

	void setLogins(const std::vector<DeviceLogin>& logins) {
		this->logins = logins;
	}
};

#endif /* USER_HPP_ */
