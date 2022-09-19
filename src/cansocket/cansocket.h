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
	NO_ERROR,
	INVALID_ARGUMENT,
	SCRIPT_NOT_FOUND,
	DEVICE_NOT_FOUND,
	SOCKETCAN_FAILED,
	SCRIPT_EXEC_FAILED,
	SOCKET_CREATION_FAILED,
	SOCKET_CLOSING_FAILED,
	SOCKET_BINDING_FAILED,
	SOCKET_CLOSED,
	SEND_ERROR,
	RECV_TIMEOUT,
	RECV_ERROR,
	INTERFACE_RETRIEVING_FAILED,
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
	int m_socket {-1};
	ifreq m_ifr;
	sockaddr_can m_addr;

	pollfd m_recvFd;
	static constexpr std::chrono::milliseconds RECV_TIMEOUT = std::chrono::milliseconds(1);

	std::mutex m_sendMutex;
	std::mutex m_recvMutex;

public:
	Socket();
	~Socket();
	Socket(const Socket& other) = delete;
	Socket& operator=(const Socket& other) = delete;

	Error connect(const std::string& interface, int bitrate);
	Error disconnect();

	Error send(const can_frame& frame);
	Error recv(can_frame& frame);
	Error recv();

private:
	Error createSocket(const std::string& interface);
	std::filesystem::path findScript(std::filesystem::path name);
};


} // namespace can


