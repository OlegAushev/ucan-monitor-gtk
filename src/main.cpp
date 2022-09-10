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


#include "ucanopen/ucanopen_client.h"


static std::thread threadMain;
static std::promise<void> signalExitMain;


std::shared_ptr<can::Socket> g_canSocket;
std::shared_ptr<ucanopen::Client> g_ucanClient;


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
	
	g_ucanClient = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x43), g_canSocket);
	g_ucanClient->serverNodes.insert({
			ucanopen::ServerNode::Name::C2000,
			ucanopen::ServerNode(ucanopen::NodeId(0x42), g_canSocket, ucanopen::OBJECT_DICTIONARY)
	});

	ucanopen::Tester ucanTester;
/*
	g_ucanClient->registerCallbackOnSendPdo(ucanopen::TpdoType::TPDO1, std::bind(&ucanopen::Tester::makeTpdo1, &ucanTester),
			std::chrono::milliseconds(500));
	g_ucanClient->registerCallbackOnSendPdo(ucanopen::TpdoType::TPDO2, std::bind(&ucanopen::Tester::makeTpdo2, &ucanTester),
			std::chrono::milliseconds(1000));
	g_ucanClient->registerCallbackOnSendPdo(ucanopen::TpdoType::TPDO3, std::bind(&ucanopen::Tester::makeTpdo3, &ucanTester),
			std::chrono::milliseconds(2000));
	g_ucanClient->registerCallbackOnSendPdo(ucanopen::TpdoType::TPDO4, std::bind(&ucanopen::Tester::makeTpdo4, &ucanTester),
			std::chrono::milliseconds(4000));
*/
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(ucanopen::TpdoType::TPDO1,
			std::bind(&ucanopen::Tester::processRpdo1, &ucanTester, std::placeholders::_1));
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(ucanopen::TpdoType::TPDO2,
			std::bind(&ucanopen::Tester::processRpdo2, &ucanTester, std::placeholders::_1));
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(ucanopen::TpdoType::TPDO3,
			std::bind(&ucanopen::Tester::processRpdo3, &ucanTester, std::placeholders::_1));
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnRecvPdo(ucanopen::TpdoType::TPDO4,
			std::bind(&ucanopen::Tester::processRpdo4, &ucanTester, std::placeholders::_1));
/*
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).enableRpdo();
	g_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).registerCallbackOnSendPdo(ucanopen::RpdoType::RPDO1,
			std::bind(&ucanopen::Tester::makeTpdo1, &ucanTester), std::chrono::milliseconds(500));
*/
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


extern "C"
void cansocket_connect()
{
	g_canSocket->connect("can0", 125000);
}


extern "C"
void cansocket_disconnect()
{
	g_canSocket->disconnect();
}
