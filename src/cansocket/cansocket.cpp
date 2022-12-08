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
	std::cout << "[cansocket] Execute system command: " << cmd << std::endl;
#endif
	int shRet = system(cmd.c_str());
	if (shRet == 0)
	{
		if (createSocket("can0") != Error::NoError)
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
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Socket creation failed." << std::endl;
#endif
		return Error::SocketCreationFailed;
	}

	std::strcpy(m_ifr.ifr_name, interface.c_str());
	if (ioctl(m_socket, SIOCGIFINDEX, &m_ifr) < 0)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Interface retrieving failed." << std::endl;
#endif
		return Error::InterfaceRetrievingFailed;
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
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Socket binding failed." << std::endl;
#endif
		return Error::SocketBindingFailed;
	}

	m_recvFd.fd = m_socket;
	m_recvFd.events = POLLIN;

#ifdef STD_COUT_ENABLED
	std::cout << "[cansocket] Socket is created." << std::endl;
#endif
	return Error::NoError;
}


///
///
///
Error Socket::connect(const std::string& interface, int bitrate)
{
	m_socket = -1;

	if (!detail::interfaceList.contains(interface)
			|| !detail::bitrateList.contains(bitrate))
	{
		return Error::InvalidArgument;
	}

	std::lock_guard<std::mutex> lock1(m_sendMutex);
	std::lock_guard<std::mutex> lock2(m_recvMutex);

	/* FIND SCRIPT */
	std::filesystem::path scriptPath = findScript("socketcan_enable.sh");
	if (scriptPath.empty())
	{
		return Error::ScriptNotFound;
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[cansocket] SocketCAN interface enabling script found: " << scriptPath << std::endl;
#endif

	/* RUN SCRIPT */
	std::string cmd = "pkexec sh " + scriptPath.string() + " " + interface + " " + std::to_string(bitrate);
#ifdef STD_COUT_ENABLED
	std::cout << "[cansocket] Execute system command: " << cmd << std::endl;
#endif
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
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] SocketCAN interface enabling failed. Error code: " << static_cast<int>(error) << std::endl;
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
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] No socket to close." << std::endl;
#endif
		return Error::NoError;
	}

	std::lock_guard<std::mutex> lock1(m_sendMutex);
	std::lock_guard<std::mutex> lock2(m_recvMutex);

	if (close(m_socket) < 0)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Socket closing failed." << std::endl;
#endif
		return Error::SocketClosingFailed;
	}
	else
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[cansocket] Socket closed." << std::endl;
#endif
		m_socket = -1;
		return Error::NoError;
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
		return Error::SocketClosed;
	}

	std::lock_guard<std::mutex> lock(m_sendMutex);

	if (::write(m_socket, &frame, sizeof(can_frame)) != sizeof(can_frame))
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
	if (m_socket < 0)
	{
		return Error::SocketClosed;
	}

	int nBytes;

	std::lock_guard<std::mutex> lock(m_recvMutex);

	int ret = poll(&m_recvFd, 1, s_recvTimeout.count());
	switch (ret)
	{
	case -1:
		return Error::RecvError;
		break;
	case 0:
		return Error::RecvTimeout;
		break;
	default:
		nBytes = ::read(m_socket, &frame, sizeof(can_frame));
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


