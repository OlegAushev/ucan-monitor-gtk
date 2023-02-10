#pragma once


#include <deque>
#include <string>
#include <mutex>
#include <cstring>
#include <iostream>


class Logger final
{
private:
	std::deque<std::string> _messages;
	std::mutex _mutex;

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
		std::cout << message << std::endl;
		std::lock_guard<std::mutex> lock(_mutex);
		_messages.push_back(message);
	}

	std::string pop()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_messages.empty())
		{
			return std::string{};
		}
		std::string message = _messages.front();
		_messages.pop_front();
		return message;
	}
};


