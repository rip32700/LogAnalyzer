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
public:
	User(const std::string& loginName, const std::vector<DeviceLogin>& logins) {
		this->mLoginName = loginName;
		this->mLogins = logins;
	}

	~User() { }

	void Add_Device_Login(const DeviceLogin& deviceLogin) {
		this->mLogins.push_back(deviceLogin);
	}

	inline friend std::ostream& operator<<(std::ostream& out, const User& user) /* output */ {
		out << "User Details: " << std::endl;
		out << "-> Login Name: " << user.mLoginName << std::endl;
		out << "-> Handy Mac: " << user.mHandyMac << std::endl;
		out << "-> Logins: " << std::endl;

		for(auto login : user.mLogins)
			out << login;

		return out;
	}

	const std::string& getLoginName() const {
		return mLoginName;
	}

	void setLoginName(const std::string& loginName) {
		mLoginName = loginName;
	}

	const std::vector<DeviceLogin>& getLogins() const {
		return mLogins;
	}

	void setLogins(const std::vector<DeviceLogin>& logins) {
		this->mLogins = logins;
	}

	const std::string& getHandyMac() const {
		return mHandyMac;
	}

	void setHandyMac(const std::string& handyMac) {
		mHandyMac = handyMac;
	}

private:
	std::string mLoginName;
	std::string mHandyMac;
	std::vector<DeviceLogin> mLogins;
};

#endif /* USER_HPP_ */
