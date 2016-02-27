/*
 * User.hpp
 *
 *  Created on: Feb 27, 2016
 *      Author: phoebus
 */

#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include <string>
#include <vector>
#include <iostream>

class Login
{
private:
	std::string login_name;
	std::string mac;
	std::string login_date;
	std::string login_time;

public:
	Login(const std::string& login_name, const std::string& mac, const std::string& login_date, const std::string& login_time)
	{
		this->login_name = login_name;
		this->mac = mac;
		this->login_date = login_date;
		this->login_time = login_time;
	}
	~Login() { }

	void Print()
	{
		std::cout << "Login Record Details: " << std::endl;
		std::cout << "---> Login Name: " << login_name << std::endl;
		std::cout << "---> MAC: " << mac << std::endl;
		std::cout << "---> Login Date: " << login_date << std::endl;
		std::cout << "---> Login Time: " << login_time << std::endl;
	}

	const std::string& getLoginDate() const {
		return login_date;
	}

	void setLoginDate(const std::string& loginDate) {
		login_date = loginDate;
	}

	const std::string& getLoginName() const {
		return login_name;
	}

	void setLoginName(const std::string& loginName) {
		login_name = loginName;
	}

	const std::string& getLoginTime() const {
		return login_time;
	}

	void setLoginTime(const std::string& loginTime) {
		login_time = loginTime;
	}

	const std::string& getMac() const {
		return mac;
	}

	void setMac(const std::string& mac) {
		this->mac = mac;
	}
};

#endif /* USER_HPP_ */
