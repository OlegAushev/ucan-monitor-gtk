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


#include <iostream>
#include <thread>
#include <future>

#include "purecan/controller/purecan_controller.h"
#include "purecan/device/purecan_device.h"

#include "atv/vcm/vcm.h"
#include "atv/leaf_inverter/leaf_inverter.h"
#include "atv/gearselector/gearselector.h"

#include "ucanopen/client/ucanopen_client.h"
#include "srmdrive/server/srmdrive_server.h"


bool backend_isReady = false;


namespace {

std::thread threadMain;
std::promise<void> signalExitMain;

}


namespace global {

std::shared_ptr<can::Socket> canSocket;
std::shared_ptr<purecan::Controller> canController;
std::shared_ptr<atv::LeafInverter> leafInverter;
std::shared_ptr<atv::GearSelector> gearSelector;

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
	global::canController = std::make_shared<purecan::Controller>(global::canSocket);


	auto creatorVcmMessage0x1D4 = []() { return atv::VehicleControlModule::instance().createMessage0x1D4(); };
	global::canController->registerTxMessage(0x1D4, "VCM Message 0x1D4", std::chrono::milliseconds(10), creatorVcmMessage0x1D4);

	auto creatorVcmMessage0x50B = []() { return atv::VehicleControlModule::instance().createMessage0x50B(); };
	global::canController->registerTxMessage(0x50B, "VCM Message 0x50B", std::chrono::milliseconds(100), creatorVcmMessage0x50B);

	auto creatorMessage0x355 = []() { return std::vector<uint8_t>{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}; };
	global::canController->registerTxMessage(0x355, "Unknown", std::chrono::milliseconds(40), creatorMessage0x355);

	global::gearSelector = std::make_shared<atv::GearSelector>();
	auto creatorMessage0x11A = []() { return global::gearSelector->createMessage0x11A(); };
	global::canController->registerTxMessage(0x11A, "Selector Message 0x11A", std::chrono::milliseconds(10), creatorMessage0x11A);

	global::leafInverter = std::make_shared<atv::LeafInverter>(global::canSocket);
	global::canController->registerDevice(global::leafInverter);






#ifdef OBSOLETE
	g_ucanClient = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), g_canSocket);
	g_srmdriveServer = std::make_shared<srmdrive::Server>(ucanopen::NodeId(0x01), g_canSocket, srmdrive::OBJECT_DICTIONARY);
	g_ucanClient->registerServer(g_srmdriveServer);

	// define and register client TPDO callbacks
	auto callbackMakeTpdo1 = []() { return g_srmdriveServer->controller.makeTpdo1(); };
	auto callbackMakeTpdo2 = []() { return g_srmdriveServer->controller.makeTpdo2(); };

	g_ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo1,
			std::chrono::milliseconds(250),
			callbackMakeTpdo1);
	g_ucanClient->registerTpdo(ucanopen::TpdoType::Tpdo2,
			std::chrono::milliseconds(100),
			callbackMakeTpdo2);

	g_ucanClient->enableSync(std::chrono::milliseconds(200));
#endif

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


