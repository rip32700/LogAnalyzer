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

class DeviceLogin
{
public:
	DeviceLogin(const std::string& mac, const time_t& timeStamp) {
		this->mMac = mac;
		this->mTimeStamp = timeStamp;
	}

	~DeviceLogin() {}

	inline friend std::ostream& operator<<(std::ostream& out, const DeviceLogin& deviceLogin) /* output */	{
		out << "---> MAC: " << deviceLogin.mMac << std::endl;
		out << "---> Login at: " << formatDate(deviceLogin.mTimeStamp) << std::endl;

		return out;
	}

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
	std::string mMac;
	time_t mTimeStamp;

	static std::string formatDate(const std::time_t time) {
		char buffer[80];
		tm* ltm = localtime(&time);
		strftime(buffer,80,"%d-%m-%Y %I:%M:%S",ltm);
		std::string formatedDate(buffer);
		return formatedDate;
	}
};



#endif /* DEVICELOGIN_HPP_ */
