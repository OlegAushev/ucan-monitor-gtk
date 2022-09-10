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
	m_socket = -1;

	// check can0: may be interface is already enabled
	/* FIND SCRIPT */
	std::filesystem::path scriptPath = findScript("socketcan_check.sh");
	if (scriptPath.empty())
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Warning: SocketCAN checking script not found." << std::endl;
#endif
		return;
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[cansocket] SocketCAN checking script found: " << scriptPath << std::endl;
#endif

	/* RUN SCRIPT */
	std::string cmd = "sh " + scriptPath.string() + " " + "can0";
#ifdef STD_COUT_ENABLED
	std::cout << "[cansocket] System cmd: " << cmd << std::endl;
#endif
	int shRet = system(cmd.c_str());
	if (shRet == 0)
	{
		if (createSocket("can0") != Error::NO_ERROR)
		{
			m_socket = -1;
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
Error Socket::createSocket(const std::string& interface)
{
	/* CREATE SOCKET */
	m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (m_socket < 0)
	{
		return Error::SOCKET_CREATION_FAILED;
	}

	std::strcpy(m_ifr.ifr_name, interface.c_str());
	if (ioctl(m_socket, SIOCGIFINDEX, &m_ifr) < 0)
	{
		return Error::INTERFACE_RETRIEVING_FAILED;
	}

	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.can_family = AF_CAN;
	m_addr.can_ifindex = m_ifr.ifr_ifindex;

	can_filter filter[1];
	filter[0].can_id = 0;
	filter[0].can_mask = 0x000;
	setsockopt(m_socket, SOL_CAN_RAW, CAN_RAW_FILTER, filter, sizeof(can_filter));

	if (bind(m_socket, (sockaddr*)&m_addr, sizeof(m_addr)) < 0)
	{
		return Error::SOCKET_BINDING_FAILED;
	}

	m_recvFd.fd = m_socket;
	m_recvFd.events = POLLIN;

	return Error::NO_ERROR;
}


///
///
///
Error Socket::connect(const std::string& interface, int bitrate)
{
	if (!detail::interfaceList.contains(interface)
			|| !detail::bitrateList.contains(bitrate))
	{
		return Error::INVALID_ARGUMENT;
	}

	std::lock_guard<std::mutex> lock1(m_sendMutex);
	std::lock_guard<std::mutex> lock2(m_recvMutex);

	/* FIND SCRIPT */
	std::filesystem::path scriptPath = findScript("socketcan_enable.sh");
	if (scriptPath.empty())
	{
		return Error::SCRIPT_NOT_FOUND;
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[cansocket] SocketCAN enabling script found: " << scriptPath << std::endl;
#endif

	/* RUN SCRIPT */
	std::string cmd = "pkexec sh " + scriptPath.string() + " " + interface + " " + std::to_string(bitrate);
#ifdef STD_COUT_ENABLED
	std::cout << "[cansocket] System cmd: " << cmd << std::endl;
#endif
	int pkexecRet = system(cmd.c_str());
	Error error;

	switch (pkexecRet)
	{
	case 0:
		error = Error::NO_ERROR;
		break;
	case 1:
		error = Error::INVALID_ARGUMENT;
		break;
	case 2:
		error = Error::DEVICE_NOT_FOUND;
		break;
	case 3:
		error = Error::SOCKETCAN_FAILED;
		break;
	default:
		error = Error::SCRIPT_EXEC_FAILED;
		break;
	}

	if (error != Error::NO_ERROR)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] CAN socket creation failed. Error code: " << static_cast<int>(error) << std::endl;
#endif
		return error;
	}

	return createSocket(interface);
}


///
///
///
Error Socket::disconnect()
{
	if (m_socket < 0)
	{
		return Error::NO_ERROR;
	}

	std::lock_guard<std::mutex> lock1(m_sendMutex);
	std::lock_guard<std::mutex> lock2(m_recvMutex);

	if (close(m_socket) < 0)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Socket closing failed." << std::endl;
#endif
		return Error::SOCKET_CLOSING_FAILED;
	}
	else
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Socket closed." << std::endl;
#endif
		m_socket = -1;
		return Error::NO_ERROR;
	}
}


///
///
///
std::filesystem::path Socket::findScript(std::filesystem::path name)
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
	if (m_socket < 0)
	{
		return Error::SOCKET_CLOSED;
	}

	std::lock_guard<std::mutex> lock(m_sendMutex);

	if (::write(m_socket, &frame, sizeof(can_frame)) != sizeof(can_frame))
	{
		return Error::SEND_ERROR;
	}
	return Error::NO_ERROR;
}


///
///
///
Error Socket::recv(can_frame& frame)
{
	if (m_socket < 0)
	{
		return Error::SOCKET_CLOSED;
	}

	int nBytes;

	std::lock_guard<std::mutex> lock(m_recvMutex);

	int ret = poll(&m_recvFd, 1, RECV_TIMEOUT.count());
	switch (ret)
	{
	case -1:
		return Error::RECV_ERROR;
		break;
	case 0:
		return Error::RECV_TIMEOUT;
		break;
	default:
		nBytes = ::read(m_socket, &frame, sizeof(can_frame));
		if (nBytes < 0)
		{
			return Error::RECV_ERROR;
		}
		else
		{
			return Error::NO_ERROR;
		}
		break;
	}
}


///
///
///
Error Socket::recv()
{
	if (m_socket < 0)
	{
		return Error::SOCKET_CLOSED;
	}

	can_frame frame;
	int nBytes;

	std::lock_guard<std::mutex> lock(m_recvMutex);

	do
	{	
		nBytes = ::recv(m_socket, &frame, sizeof(can_frame), MSG_PEEK | MSG_DONTWAIT);	// non-blocking read
												// check if there is available CAN frame
		if (nBytes == sizeof(can_frame))
		{
			nBytes = ::recv(m_socket, &frame, sizeof(can_frame), MSG_DONTWAIT);	// non-blocking read
			if (nBytes < 0)
			{
				return Error::RECV_ERROR;
			}

			// TODO emit signal(frame)
		}
		else if (nBytes < 0 && errno != EAGAIN)	// error was caused not by absence of CAN frame
		{
			return Error::RECV_ERROR;
		}
	} while (nBytes == sizeof(can_frame));

	return Error::NO_ERROR;	
}


} // namespace can

