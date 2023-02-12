#include "cansocket.h"


namespace can {

Socket::Socket()
{
	// check can0: may be interface is already enabled
	/* FIND SCRIPT */
	std::stringstream msg;
	msg << "[cansocket] Searching for SocketCAN checking script... ";
	std::filesystem::path script_path = _find_script("socketcan_check.sh");
	if (script_path.empty())
	{
		msg << "WARNING: not found.";
		return;
	}
	msg << "found: " << script_path;
	Log() << msg;
	msg.str("");

	/* RUN SCRIPT */
	std::string cmd = "sh " + script_path.string() + " " + "can0";
	msg << "[cansocket] Checking can0, executing system command: \"" << cmd << "\"";
	Log() << msg;

	int script_retval = system(cmd.c_str());
	if (script_retval == 0)
	{
		if (_create_socket("can0") != Error::no_error)
		{
			_socket = -1;
		}
	}
}


Socket::~Socket()
{
	disconnect();
}


Error Socket::_create_socket(const std::string& interface)
{
	/* CREATE SOCKET */
	Log() << "[cansocket] Creating socket...";
	_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (_socket < 0)
	{
		Log() << "[cansocket] ERROR: socket creation failed.";
		return Error::socket_creation_failed;
	}

	std::strcpy(_ifr.ifr_name, interface.c_str());
	if (ioctl(_socket, SIOCGIFINDEX, &_ifr) < 0)
	{
		Log() << "[cansocket] ERROR: interface retrieving failed.";
		return Error::interface_retrieving_failed;
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
		Log() << "[cansocket] ERROR: socket binding failed.";
		return Error::socket_binding_failed;
	}

	_recv_fd.fd = _socket;
	_recv_fd.events = POLLIN;

	Log() << "[cansocket] Socket created.";
	return Error::no_error;
}


Error Socket::connect(const std::string& interface, int bitrate)
{
	_socket = -1;

	if (!detail::interface_list.contains(interface)
			|| !detail::bitrate_list.contains(bitrate))
	{
		return Error::invalid_argument;
	}

	std::lock_guard<std::mutex> lock1(_send_mutex);
	std::lock_guard<std::mutex> lock2(_recv_mutex);

	/* FIND SCRIPT */
	std::stringstream msg;
	msg << "[cansocket] Searching for SocketCAN enabling script... ";
	std::filesystem::path script_path = _find_script("socketcan_enable.sh");
	if (script_path.empty())
	{
		msg << "ERROR: not found.";
		return Error::script_not_found;
	}
	msg << "found: " << script_path;
	Log() << msg;
	msg.str("");

	/* RUN SCRIPT */
	std::string cmd = "pkexec sh " + script_path.string() + " " + interface + " " + std::to_string(bitrate);
	msg << "[cansocket] Enabling " << interface << ", executing system command: \"" << cmd << "\"";
	Log() << msg;
	msg.str("");

	int pkexec_retval = system(cmd.c_str());
	Error error;

	switch (pkexec_retval)
	{
		case 0:
			error = Error::no_error;
			break;
		case 1:
			error = Error::invalid_argument;
			break;
		case 2:
			error = Error::device_not_found;
			break;
		case 3:
			error = Error::socket_can_failed;
			break;
		default:
			error = Error::script_exec_failed;
			break;
	}

	if (error != Error::no_error)
	{
		msg << "[cansocket] SocketCAN interface enabling failed. Error code: " << static_cast<int>(error);
		Log() << msg;
		return error;
	}

	return _create_socket(interface);
}


Error Socket::disconnect()
{
	if (_socket < 0)
	{
		Log() << "[cansocket] No socket to close.";
		return Error::no_error;
	}

	std::lock_guard<std::mutex> lock1(_send_mutex);
	std::lock_guard<std::mutex> lock2(_recv_mutex);

	if (close(_socket) < 0)
	{
		Log() << "[cansocket] ERROR: socket closing failed.";
		return Error::socket_closing_failed;
	}
	else
	{
		Log() << "[cansocket] Socket closed.";
		_socket = -1;
		return Error::no_error;
	}
}


std::filesystem::path Socket::_find_script(std::filesystem::path name)
{
	std::filesystem::path script_path;
	for (auto loc : can::detail::scripts_location_list)
	{
		auto absolute_path = std::filesystem::absolute(loc/name);
		if (std::filesystem::exists(absolute_path))
		{
			script_path = std::filesystem::canonical(loc/name);
		}
	}
	return script_path;
}


Error Socket::send(const can_frame& frame)
{
	if (_socket < 0)
	{
		return Error::socket_closed;
	}

	std::lock_guard<std::mutex> lock(_send_mutex);

	if (::write(_socket, &frame, sizeof(can_frame)) != sizeof(can_frame))
	{
		return Error::send_error;
	}
	return Error::no_error;
}


Error Socket::recv(can_frame& frame)
{
	if (_socket < 0)
	{
		return Error::socket_closed;
	}

	int byte_count;

	std::lock_guard<std::mutex> lock(_recv_mutex);

	int ret = poll(&_recv_fd, 1, _recv_timeout.count());
	switch (ret)
	{
		case -1:
			return Error::recv_error;
			break;
		case 0:
			return Error::recv_timeout;
			break;
		default:
			byte_count = ::read(_socket, &frame, sizeof(can_frame));
			if (byte_count < 0)
			{
				return Error::recv_error;
			}
			else
			{
				return Error::no_error;
			}
			break;
	}
}

} // namespace can

