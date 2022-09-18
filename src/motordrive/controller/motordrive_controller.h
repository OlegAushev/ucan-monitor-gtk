/**
 * @file motordrive_controller.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "ucanopen/ucanopen_client.h"


namespace motordrive {


class Controller
{
private:
	std::shared_ptr<ucanopen::Client> m_ucanClient;

	bool m_runFlag = false;
	bool m_emergencyFlag = false;

	float m_torquePuRef = 0;
	float m_speedRef = 0;

public:
	Controller(std::shared_ptr<ucanopen::Client> ucanClient);
	
	void powerUp();
	void powerDown();
};



} // namespace motordrive


