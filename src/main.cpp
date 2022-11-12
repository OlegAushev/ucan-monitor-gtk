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
#include "motordrive/controller/motordrive_controller.h"
#include "motordrive/observer/motordrive_observer.h"


static std::thread threadMain;
static std::promise<void> signalExitMain;

bool g_isBackendReady = false;
std::shared_ptr<can::Socket> g_canSocket;
std::shared_ptr<ucanopen::Client> g_ucanClient;
std::shared_ptr<motordrive::Controller> g_motordriveController;
std::shared_ptr<motordrive::Observer> g_motordriveObserver;


/**
 * @brief 
 * 
 * @param futureExit 
 * @return int 
 */
extern "C" 
int main_loop(std::future<void> futureExit)
{
#ifdef STD_COUT_ENABLED
	std::cout << "[cpp] Main loop thread has started. Thread id: " << std::this_thread::get_id() << std::endl;
#endif

	g_canSocket = std::make_shared<can::Socket>();

	g_ucanClient = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), g_canSocket);
	g_ucanClient->serverNodes.insert({
			ucanopen::ServerNode::Name::C2000,
			ucanopen::ServerNode(ucanopen::NodeId(0x01), g_canSocket, ucanopen::OBJECT_DICTIONARY)
	});

	g_motordriveController = std::make_shared<motordrive::Controller>(g_ucanClient);
	g_motordriveObserver = std::make_shared<motordrive::Observer>(g_ucanClient);

	// define and register TPDO callbacks
	auto callbackMakeTpdo1 = []() { return g_motordriveController->makeTpdo1(); };
	auto callbackMakeTpdo2 = []() { return g_motordriveController->makeTpdo2(); };

	g_ucanClient->registerCallbackOnSendPdo(ucanopen::TpdoType::TPDO1,
			callbackMakeTpdo1,
			std::chrono::milliseconds(250));
	g_ucanClient->registerCallbackOnSendPdo(ucanopen::TpdoType::TPDO2,
			callbackMakeTpdo2,
			std::chrono::milliseconds(100));

	// define and register RPDO callbacks
	ucanopen::Tester ucanTester;
	auto callbackProcessRpdo1 = [&ucanTester](std::array<uint8_t, 8> data) { return ucanTester.processRpdo1(data); };
	auto callbackProcessRpdo2 = [&ucanTester](std::array<uint8_t, 8> data) { return ucanTester.processRpdo2(data); };
	auto callbackProcessRpdo3 = [&ucanTester](std::array<uint8_t, 8> data) { return ucanTester.processRpdo3(data); };
	auto callbackProcessRpdo4 = [&ucanTester](std::array<uint8_t, 8> data) { return ucanTester.processRpdo4(data); };

	/*g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(
			ucanopen::TpdoType::TPDO1,
			callbackProcessRpdo1);
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(
			ucanopen::TpdoType::TPDO2,
			callbackProcessRpdo2);
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(
			ucanopen::TpdoType::TPDO3,
			callbackProcessRpdo3);
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(
			ucanopen::TpdoType::TPDO4,
			callbackProcessRpdo4);*/

	// define and register SDO callback
	auto callbackProcessSdo = [](ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionaryType::const_iterator entryIt,
			ucanopen::CobSdoData data)
	{
		return g_motordriveObserver->processSdo(sdoType, entryIt, data);
	};
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvSdo(callbackProcessSdo);

/*
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).enableRpdo();
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnSendPdo(ucanopen::RpdoType::RPDO1,
			std::bind(&ucanopen::Tester::makeTpdo1, &ucanTester), std::chrono::milliseconds(500));
*/

#ifdef STD_COUT_ENABLED
	std::cout << "[cpp] Backend is ready." << std::endl;
#endif
	g_isBackendReady = true;

	while (futureExit.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout)
	{
		
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[cpp] Main loop thread has stopped." << std::endl;
#endif
	return 0;
}


/**
 * @brief 
 * 
 * @return int 
 */
extern "C"
int main_enter()
{
#ifdef STD_COUT_ENABLED
	std::cout << "[cpp] Thread id: " << std::this_thread::get_id() << std::endl;
	std::cout << "[cpp] Starting new thread for main loop..." << std::endl;
#endif
	std::future<void> futureExit = signalExitMain.get_future();
	threadMain = std::thread(main_loop, std::move(futureExit));
	return 0;
}


/**
 * @brief 
 * 
 */
extern "C"
void main_exit()
{
#ifdef STD_COUT_ENABLED
	std::cout << "[cpp] Sending signal to main loop thread to stop..." << std::endl;
#endif
	signalExitMain.set_value();
	threadMain.join();
}


