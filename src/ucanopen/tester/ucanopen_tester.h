/**
 * @file ucanopen_tester.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <iostream>
#include <iomanip>
#include <chrono>

#include "../ucanopen_def.h"


namespace ucanopen {


class Tester
{
private:
	std::chrono::time_point<std::chrono::steady_clock> m_initTimepoint;
public:
	Tester()
	{
		m_initTimepoint = std::chrono::steady_clock::now();
	}

	auto timestamp()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_initTimepoint).count();
	}

	std::array<uint8_t, 8> makeTpdo1()
	{
		std::cout << "[ucanopen] TPDO1"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< std::endl;
		return {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF};
	}

	std::array<uint8_t, 8> makeTpdo2()
	{
		std::cout << "[ucanopen] TPDO2"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< std::endl;
		return {0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0xFF};
	}

	std::array<uint8_t, 8> makeTpdo3()
	{
		std::cout << "[ucanopen] TPDO3"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< std::endl;
		return {0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xFF};
	}

	std::array<uint8_t, 8> makeTpdo4()
	{
		std::cout << "[ucanopen] TPDO4"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< std::endl;
		return {0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0xFF};
	}

	void processRpdo1(std::array<uint8_t, 8> data)
	{
		std::cout << "[ucanopen] RPDO1"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< " | data: ";

		for (uint8_t val : data)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << int(val) << " ";
		}
		std::cout << std::endl;
	}

	void processRpdo2(std::array<uint8_t, 8> data)
	{
		std::cout << "[ucanopen] RPDO2"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< " | data: ";

		for (uint8_t val : data)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << int(val) << " ";
		}
		std::cout << std::endl;
	}

	void processRpdo3(std::array<uint8_t, 8> data)
	{
		std::cout << "[ucanopen] RPDO3"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< " | data: ";

		for (uint8_t val : data)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << int(val) << " ";
		}
		std::cout << std::endl;
	}

	void processRpdo4(std::array<uint8_t, 8> data)
	{
		std::cout << "[ucanopen] RPDO4"
				<< " | timestamp: " << std::dec << std::setw(7) << timestamp() << "ms"
				<< " | thread: " << std::this_thread::get_id()
				<< " | data: ";

		for (uint8_t val : data)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << int(val) << " ";
		}
		std::cout << std::endl;
	}
};


} // namespace ucanopen


