/**
 * @file cansocket.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "cansocket.h"


namespace can {


///
///
///
Socket::Socket()
{
	// check can0: may be interface is already enabled
	/* FIND SCRIPT */
	std::cout << "[cansocket] Searching for SocketCAN checking script... ";
	std::filesystem::path scriptPath = _findScript("socketcan_check.sh");
	if (scriptPath.empty())
	{
		std::cout << "WARNING: not found." << std::endl;
		return;
	}
	std::cout << "found: " << scriptPath << std::endl;

	/* RUN SCRIPT */
	std::string cmd = "sh " + scriptPath.string() + " " + "can0";
	std::cout << "[cansocket] Checking can0, executing system command: \"" << cmd << "\"" << std::endl;

	int shRet = system(cmd.c_str());
	if (shRet == 0)
	{
		if (_createSocket("can0") != Error::NoError)
		{
			_socket = -1;
		}
	}
}


///
///
///
Socket::~Socket()
{
	disconnect();
}


///
///
///
Error Socket::_createSocket(const std::string& interface)
{
	/* CREATE SOCKET */
	std::cout << "[cansocket] Creating socket..." << std::endl;
	_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (_socket < 0)
	{
		std::cout << "[cansocket] ERROR: socket creation failed." << std::endl;
		return Error::SocketCreationFailed;
	}

	std::strcpy(_ifr.ifr_name, interface.c_str());
	if (ioctl(_socket, SIOCGIFINDEX, &_ifr) < 0)
	{
		std::cout << "[cansocket] ERROR: interface retrieving failed." << std::endl;
		return Error::InterfaceRetrievingFailed;
	}

	memset(&_addr, 0, sizeof(_addr));
	_addr.can_family = AF_CAN;
	_addr.can_ifindex = _ifr.ifr_ifindex;

	can_filter filter[1];
	filter[0].can_id = 0;
	filter[0].can_mask = 0x000;
	setsockopt(_socket, SOL_CAN_RAW, CAN_RAW_FILTER, filter, sizeof(can_filter));

	if (bind(_socket, (sockaddr*)&_addr, sizeof(_addr)) < 0)
	{
		std::cout << "[cansocket] ERROR: socket binding failed." << std::endl;
		return Error::SocketBindingFailed;
	}

	_recvFd.fd = _socket;
	_recvFd.events = POLLIN;

	std::cout << "[cansocket] Socket created." << std::endl;
	return Error::NoError;
}


///
///
///
Error Socket::connect(const std::string& interface, int bitrate)
{
	_socket = -1;

	if (!detail::interfaceList.contains(interface)
			|| !detail::bitrateList.contains(bitrate))
	{
		return Error::InvalidArgument;
	}

	std::lock_guard<std::mutex> lock1(_sendMutex);
	std::lock_guard<std::mutex> lock2(_recvMutex);

	/* FIND SCRIPT */
	std::cout << "[cansocket] Searching for SocketCAN enabling script... ";
	std::filesystem::path scriptPath = _findScript("socketcan_enable.sh");
	if (scriptPath.empty())
	{
		std::cout << "ERROR: not found." << std::endl;
		return Error::ScriptNotFound;
	}
	std::cout << "found: " << scriptPath << std::endl;

	/* RUN SCRIPT */
	std::string cmd = "pkexec sh " + scriptPath.string() + " " + interface + " " + std::to_string(bitrate);
	std::cout << "[cansocket] Enabling " << interface << ", executing system command: \"" << cmd << "\"" << std::endl;

	int pkexecRet = system(cmd.c_str());
	Error error;

	switch (pkexecRet)
	{
	case 0:
		error = Error::NoError;
		break;
	case 1:
		error = Error::InvalidArgument;
		break;
	case 2:
		error = Error::DeviceNotFound;
		break;
	case 3:
		error = Error::SocketcanFailed;
		break;
	default:
		error = Error::ScriptExecFailed;
		break;
	}

	if (error != Error::NoError)
	{
		std::cout << "[cansocket] SocketCAN interface enabling failed. Error code: " << static_cast<int>(error) << std::endl;
		return error;
	}

	return _createSocket(interface);
}


///
///
///
Error Socket::disconnect()
{
	if (_socket < 0)
	{
		std::cout << "[cansocket] No socket to close." << std::endl;
		return Error::NoError;
	}

	std::lock_guard<std::mutex> lock1(_sendMutex);
	std::lock_guard<std::mutex> lock2(_recvMutex);

	if (close(_socket) < 0)
	{
		std::cout << "[cansocket] ERROR: socket closing failed." << std::endl;
		return Error::SocketClosingFailed;
	}
	else
	{
		std::cout << "[cansocket] Socket closed." << std::endl;
		_socket = -1;
		return Error::NoError;
	}
}


///
///
///
std::filesystem::path Socket::_findScript(std::filesystem::path name)
{
	std::filesystem::path scriptPath;
	for (auto loc : can::detail::scriptsLocationList)
	{
		auto absolutePath = std::filesystem::absolute(loc/name);
		if (std::filesystem::exists(absolutePath))
		{
			scriptPath = std::filesystem::canonical(loc/name);
		}
	}
	return scriptPath;
}


///
///
///
Error Socket::send(const can_frame& frame)
{
	if (_socket < 0)
	{
		return Error::SocketClosed;
	}

	std::lock_guard<std::mutex> lock(_sendMutex);

	if (::write(_socket, &frame, sizeof(can_frame)) != sizeof(can_frame))
	{
		return Error::SendError;
	}
	return Error::NoError;
}


///
///
///
Error Socket::recv(can_frame& frame)
{
	if (_socket < 0)
	{
		return Error::SocketClosed;
	}

	int nBytes;

	std::lock_guard<std::mutex> lock(_recvMutex);

	int ret = poll(&_recvFd, 1, _recvTimeout.count());
	switch (ret)
	{
	case -1:
		return Error::RecvError;
		break;
	case 0:
		return Error::RecvTimeout;
		break;
	default:
		nBytes = ::read(_socket, &frame, sizeof(can_frame));
		if (nBytes < 0)
		{
			return Error::RecvError;
		}
		else
		{
			return Error::NoError;
		}
		break;
	}
}


} // namespace can


