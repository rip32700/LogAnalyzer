/*
 * DeviceLogin.hpp
 *
 *  Created on: Feb 29, 2016
 *      Author: phoebus
 */

#ifndef DEVICELOGIN_HPP_
#define DEVICELOGIN_HPP_


class DeviceLogin
{
private:
	std::string mac;
	std::string login_time;

public:
	DeviceLogin(const std::string& mac, const std::string& login_time)
	{
		this->mac = mac;
		this->login_time = login_time;
	}
	~DeviceLogin() {}

	void Print()
	{
		//std::cout << "Device Login Details: " << std::endl;
		std::cout << "---> MAC: " << mac << std::endl;
		std::cout << "---> Login at : " << login_time << std::endl;
	}

	const std::string& getMac() const {
		return mac;
	}

	void setMac(const std::string& mac) {
		this->mac = mac;
	}

	const std::string& getLoginTime() const {
		return login_time;
	}

	void setLoginTime(const std::string& loginTime) {
		login_time = loginTime;
	}
};



#endif /* DEVICELOGIN_HPP_ */
