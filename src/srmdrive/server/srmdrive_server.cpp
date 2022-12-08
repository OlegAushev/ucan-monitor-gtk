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


Server::Server(const std::string& name,
		ucanopen::NodeId nodeId,
		std::shared_ptr<can::Socket> socket,
		const ucanopen::ObjectDictionaryType& dictionary)
	: IServer(name, nodeId, socket, dictionary)
	, controller(this)
	, observer(this)
{
	registerTpdo(ucanopen::TpdoType::Tpdo1, std::chrono::milliseconds(200));
	registerTpdo(ucanopen::TpdoType::Tpdo2, std::chrono::milliseconds(1200));
	registerTpdo(ucanopen::TpdoType::Tpdo3, std::chrono::milliseconds(200));
	registerTpdo(ucanopen::TpdoType::Tpdo4, std::chrono::milliseconds(200));
}


}


