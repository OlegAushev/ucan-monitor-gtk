#pragma once


#include <deque>
#include <string>
#include <mutex>
#include <cstring>
#include <iostream>
#include <syncstream>


class Log final
{
	//friend Log& operator<<(Log&& log, const std::string& s);
	//friend void operator<<(Log&& log, const std::stringstream& ss);
private:
	static inline std::deque<std::string> _messages;
	static inline std::mutex _mutex;

public:
	Log() = default;
	Log(const Log& other) = delete;
	Log& operator=(const Log& other) = delete;

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

	template <typename T>
	Log& operator<<(const T& msg)
	{
		std::cout << msg;
		std::lock_guard<std::mutex> lock(_mutex);
		//_messages.push_back(s);
		return *this;
	}

	Log& operator<<(const std::stringstream& ss)
	{
		std::cout << ss.str();
		std::lock_guard<std::mutex> lock(_mutex);
		//log._messages.push_back(ss.str());
		return *this;
	}
};

