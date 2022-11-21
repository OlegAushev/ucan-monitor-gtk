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
#include "srmdrive/server/srmdrive_server.h"


bool backend_isReady = false;


static std::thread threadMain;
static std::promise<void> signalExitMain;


namespace global {

std::shared_ptr<can::Socket> canSocket;
std::shared_ptr<ucanopen::Client> ucanClient;
std::shared_ptr<srmdrive::Server> srmdriveServer;

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
#ifdef STD_COUT_ENABLED
	std::cout << "[backend] Main loop thread has started. Thread id: " << std::this_thread::get_id() << std::endl;
#endif

	global::canSocket = std::make_shared<can::Socket>();

	global::ucanClient = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), global::canSocket);
	global::srmdriveServer = std::make_shared<srmdrive::Server>(ucanopen::NodeId(0x01), global::canSocket, srmdrive::OBJECT_DICTIONARY);
	global::ucanClient->registerServer(global::srmdriveServer);

	// define and register client TPDO callbacks
	auto callbackMakeTpdo1 = []() { return global::srmdriveServer->controller.makeTpdo1(); };
	auto callbackMakeTpdo2 = []() { return global::srmdriveServer->controller.makeTpdo2(); };

	global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo1,
			std::chrono::milliseconds(250),
			callbackMakeTpdo1);
	global::ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo2,
			std::chrono::milliseconds(100),
			callbackMakeTpdo2);

	global::ucanClient->enableSync(std::chrono::milliseconds(200));

#ifdef STD_COUT_ENABLED
	std::cout << "[backend] Backend is ready." << std::endl;
#endif
	backend_isReady = true;

	while (futureExit.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout)
	{
		
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[backend] Main loop thread has stopped." << std::endl;
#endif
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
#ifdef STD_COUT_ENABLED
	std::cout << "[backend] Thread id: " << std::this_thread::get_id() << std::endl;
	std::cout << "[backend] Starting new thread for main loop..." << std::endl;
#endif
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
#ifdef STD_COUT_ENABLED
	std::cout << "[backend] Sending signal to main loop thread to stop..." << std::endl;
#endif
	signalExitMain.set_value();
	threadMain.join();
}


