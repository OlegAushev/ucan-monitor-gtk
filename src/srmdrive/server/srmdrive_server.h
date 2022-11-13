/**
 * @file srmdrive_server.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "ucanopen/server/ucanopen_server.h"
#include "objectdictionary/objectdictionary.h"
#include "../controller/srmdrive_controller.h"
#include "../observer/srmdrive_observer.h"


namespace srmdrive {


class Server : public ucanopen::IServer
{
private:


public:
	Controller controller;
	Observer observer;

	Server(ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket, const ucanopen::ObjectDictionaryType& dictionary)
		: IServer(nodeId, socket, dictionary)
		, controller(this)
		, observer(this)
	{

	}

};


} // namespace srmdrive

