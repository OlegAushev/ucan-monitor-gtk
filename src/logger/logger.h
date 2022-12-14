/**
 * @file logger.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <deque>
#include <string>
#include <mutex>
#include <cstring>


class Logger final
{
private:
	std::deque<std::string> m_messages;
	std::mutex m_mutex;

	Logger() = default;
public:
	Logger(const Logger& other) = delete;
	Logger& operator=(const Logger& other) = delete;

	static Logger& instance()
	{
		static Logger logger;
		return logger;
	}

	void add(std::string message)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_messages.push_back(message);
	}

	std::string pop()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_messages.empty())
		{
			return std::string{};
		}
		std::string message = m_messages.front();
		m_messages.pop_front();
		return message;
	}
};


