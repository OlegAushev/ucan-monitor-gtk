/**
 * @file cansocket.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <poll.h>

#include <iostream>
#include <string>
#include <set>
#include <filesystem>
#include <thread>
#include <mutex>
#include <chrono>


namespace can {


enum class Error
{
	NoError,
	InvalidArgument,
	ScriptNotFound,
	DeviceNotFound,
	SocketcanFailed,
	ScriptExecFailed,
	SocketCreationFailed,
	SocketClosingFailed,
	SocketBindingFailed,
	SocketClosed,
	SendError,
	RecvTimeout,
	RecvError,
	InterfaceRetrievingFailed,
};


namespace detail {


const std::set<std::string> interfaceList = {"can0", "can1"};
const std::set<int> bitrateList = {125'000, 250'000, 500'000, 1'000'000};
const std::set<std::filesystem::path> scriptsLocationList = {
	"",
	"scripts",
	"..",
	"../scripts"
};


} // namespace detail


/**
 * @brief 
 * 
 */
class Socket
{
private:
	int _socket{-1};
	ifreq _ifr;
	sockaddr_can _addr;

	pollfd _recvFd;
	static constexpr std::chrono::milliseconds _recvTimeout = std::chrono::milliseconds(1);

	std::mutex _sendMutex;
	std::mutex _recvMutex;

public:
	Socket();
	~Socket();
	Socket(const Socket& other) = delete;
	Socket& operator=(const Socket& other) = delete;

	Error connect(const std::string& interface, int bitrate);
	Error disconnect();

	Error send(const can_frame& frame);
	Error recv(can_frame& frame);

private:
	Error _createSocket(const std::string& interface);
	std::filesystem::path _findScript(std::filesystem::path name);
};


} // namespace can


