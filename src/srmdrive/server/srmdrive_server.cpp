/**
 * @file srmdrive_server.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "srmdrive_server.h"


namespace srmdrive {


Server::Server(ucanopen::NodeId nodeId,
		std::shared_ptr<can::Socket> socket,
		const ucanopen::ObjectDictionaryType& dictionary)
	: IServer(nodeId, socket, dictionary)
	, controller(this)
	, observer(this)
{

}


}


