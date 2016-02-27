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
#include "device.hpp"

class User
{
private:
	std::string login_name;
	std::vector<Device> macs;

public:
	User(const std::string& login_name, const std::vector<Device>& macs)
	{
		this->login_name = login_name;
		this->macs = macs;
	}
	~User() { }

	void Print()
	{
		std::cout << "Device Details: " << std::endl;
		std::cout << "---> MACs: " << login_name << std::endl;
		for(auto mac : macs)
		{
			std::cout << "---> *** ";
			mac.Print();
			std::cout << std::endl;
		}
	}
};

#endif /* USER_HPP_ */
