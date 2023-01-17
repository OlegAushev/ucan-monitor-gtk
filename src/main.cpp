/**
 * @file main.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucanopen/client/ucanopen_client.h"
#include "ucanopen_devices/srmdrive/server/srmdrive_server.h"
#include "ucanopen_devices/launchpad/server/launchpad_server.h"
#include "ucanopen_devices/bmsmain/server/bmsmain_server.h"
#include "gnuplotter/gnuplotter.h"


bool backend_is_ready = false;
const char* backend_ucanopen_server;
const char* backend_ucanopen_server_list[3] = {"SRM Drive", "LaunchPad", "BMS Main"};
const char* backend_ucanopen_server_config_category;


namespace {

std::thread threadMain;
std::promise<void> signalExitMain;

}


namespace global {

std::shared_ptr<can::Socket> canSocket;
std::shared_ptr<ucanopen::Client> ucanClient;
std::shared_ptr<srmdrive::Server> srmdriveServer;
std::shared_ptr<launchpad::Server> launchpadServer;
std::shared_ptr<bmsmain::Server> bmsmainServer;

}



/**
 * @brief 
 * 
 * @param futureExit 
 * @return int 
 */
extern "C" 
int backend_main_loop(std::future<void> futureExit)
{
	std::cout << "[backend] Main loop thread started. Thread id: " << std::this_thread::get_id() << std::endl;

	global::canSocket = std::make_shared<can::Socket>();

	global::ucanClient = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), global::canSocket);

	std::string serverName(backend_ucanopen_server);
	if (serverName == "SRM Drive")
	{
		global::srmdriveServer = std::make_shared<srmdrive::Server>("SRM Drive", ucanopen::NodeId(0x01), global::canSocket);
		global::ucanClient->registerServer(global::srmdriveServer);
		
		auto callbackCreateTpdo1 = [](){ return global::srmdriveServer->controller.makeTpdo1(); };
		auto callbackCreateTpdo2 = [](){ return global::srmdriveServer->controller.makeTpdo2(); };

		global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo1, std::chrono::milliseconds(250), callbackCreateTpdo1);
		global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo2, std::chrono::milliseconds(100), callbackCreateTpdo2);
	}
	else if (serverName == "LaunchPad")
	{
		global::launchpadServer = std::make_shared<launchpad::Server>("LaunchPad", ucanopen::NodeId(0x142), global::canSocket);
		global::ucanClient->registerServer(global::launchpadServer);

		auto callbackCreateTpdo1 = [](){ return global::launchpadServer->createClientTpdo1(); };
		auto callbackCreateTpdo2 = [](){ return global::launchpadServer->createClientTpdo2(); };
		auto callbackCreateTpdo3 = [](){ return global::launchpadServer->createClientTpdo3(); };
		auto callbackCreateTpdo4 = [](){ return global::launchpadServer->createClientTpdo4(); };

		global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo1, std::chrono::milliseconds(50), callbackCreateTpdo1);
		global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo2, std::chrono::milliseconds(100), callbackCreateTpdo2);
		global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo3, std::chrono::milliseconds(250), callbackCreateTpdo3);
		global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo4, std::chrono::milliseconds(1000), callbackCreateTpdo4);
	}
	else if (serverName == "BMS Main")
	{
		global::bmsmainServer = std::make_shared<bmsmain::Server>("BMS Main", ucanopen::NodeId(0x20), global::canSocket);
		global::ucanClient->registerServer(global::bmsmainServer);
	}

	backend_ucanopen_server_config_category = global::ucanClient->server(serverName)->configService.configCategory.data();

	std::cout << "[backend] Backend ready." << std::endl;
	backend_is_ready = true;

	while (futureExit.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout)
	{

	}

	std::cout << "[backend] Main loop thread stopped." << std::endl;
	return 0;
}


/**
 * @brief 
 * 
 * @return int 
 */
extern "C"
int backend_main_enter()
{
	std::cout << "[backend] Thread id: " << std::this_thread::get_id() << std::endl;
	std::cout << "[backend] Starting new thread for main loop..." << std::endl;

	std::future<void> futureExit = signalExitMain.get_future();
	threadMain = std::thread(backend_main_loop, std::move(futureExit));
	return 0;
}


/**
 * @brief 
 * 
 */
extern "C"
void backend_main_exit()
{
	std::cout << "[backend] Sending signal to main loop thread to stop..." << std::endl;

	signalExitMain.set_value();
	threadMain.join();
}


