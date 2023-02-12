#pragma once


#include <deque>
#include <string>
#include <mutex>
#include <cstring>
#include <iostream>
#include <syncstream>


class Log final
{
	friend void operator<<(Log&& log, const std::string& s);
	friend void operator<<(Log&& log, const std::stringstream& ss);
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
};


inline void operator<<(Log&& log, const std::string& s)
{
	std::osyncstream(std::cout) << s << '\n';
	std::lock_guard<std::mutex> lock(log._mutex);
	log._messages.push_back(s);
}


inline void operator<<(Log&& log, const std::stringstream& ss)
{
	std::osyncstream(std::cout) << ss.str() << '\n';
	std::lock_guard<std::mutex> lock(log._mutex);
	log._messages.push_back(ss.str());
}