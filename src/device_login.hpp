/*
 * DeviceLogin.hpp
 *
 *  Created on: Feb 29, 2016
 *      Author: phoebus
 */

#ifndef DEVICELOGIN_HPP_
#define DEVICELOGIN_HPP_

#include <ctime>
#include <string>
#include <iostream>

/**
  * @author: prieger
  * Device Login object
  * - represents a login of an device into the system
  * - contains the device's mac address and the timestamp of the login
  */

class DeviceLogin
{
public:

	/* CONSTRUCTOR and DESTRUCTOR */

	DeviceLogin(const std::string& mac, const time_t& timeStamp) {
		this->mMac = mac;
		this->mTimeStamp = timeStamp;
	}

	~DeviceLogin() {}

	/* OPERATORS */

	inline friend std::ostream& operator<<(std::ostream& out, const DeviceLogin& deviceLogin) /* output */	{
		out << "---> MAC: " << deviceLogin.mMac << std::endl;
		out << "---> Login at: " << formatDate(deviceLogin.mTimeStamp) << std::endl;

		return out;
	}

	/* GETTERS AND SETTERS */

	const std::string& getMac() const {
		return mMac;
	}

	void setMac(const std::string& mac) {
		this->mMac = mac;
	}

	time_t getTimeStamp() const {
		return mTimeStamp;
	}

	void setTimeStamp(time_t timeStamp) {
		mTimeStamp = timeStamp;
	}

private:
	
	/* MEMBER ATTRIBUTES */

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



#endif /* DEVICELOGIN_HPP_ */
