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
#include <ctime>

/**
 * @author: prieger
 * Login object
 * - represents a Login activity on the system extracted from the logs
 * - contains the login-name, mac-address and timestamp
 */

class Login
{
public:

	/* CONSTRUCTOR and DESTRUCTOR */

	Login(const std::string& loginName, const std::string& mac, const time_t& timeStamp) {
		this->mLoginName = loginName;
		this->mMac = mac;
		this->mTimeStamp = timeStamp;
	}

	~Login() { }

	/* OPERATORS */

	inline friend std::ostream& operator<<(std::ostream& out, const Login& login) /* output */ {
		out << "Login Record Details: " << std::endl;
		out << "-> Login Name: " << login.mLoginName << std::endl;
		out << "-> MAC: " << login.mMac << std::endl;
		out << "-> Login Date: " << formatDate(login.mTimeStamp) << std::endl;

		return out;
	}

 	/* GETTERS AND SETTERS */

	const std::string& getLoginName() const {
		return mLoginName;
	}

	void setLoginName(const std::string& loginName) {
		mLoginName = loginName;
	}

	const std::string& getMac() const {
		return mMac;
	}

	void setMac(const std::string& mac) {
		mMac = mac;
	}

	time_t getTimeStamp() const {
		return mTimeStamp;
	}

	void setTimeStamp(time_t timeStamp) {
		mTimeStamp = timeStamp;
	}

private:

	/* MEMBER ATTRIBUTES */

	std::string mLoginName;
	std::string mMac;
	time_t mTimeStamp;

	/* STATIC HELPER METHODS */
	
	static std::string formatDate(const std::time_t time) {
		char buffer[80];
		tm* ltm = localtime(&time);
		strftime(buffer,80,"%d-%m-%Y %I:%M:%S",ltm);
		std::string formatedDate(buffer);
		return formatedDate;
	}
};



#endif /* USER_HPP_ */
