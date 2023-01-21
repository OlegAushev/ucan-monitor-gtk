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

std::thread thread_main;
std::promise<void> signal_exit_main;

}


namespace global {

std::shared_ptr<can::Socket> can_socket;
std::shared_ptr<ucanopen::Client> ucanopen_client;
std::shared_ptr<srmdrive::Server> srmdrive_server;
std::shared_ptr<launchpad::Server> launchpad_server;
std::shared_ptr<bmsmain::Server> bmsmain_server;

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

	global::can_socket = std::make_shared<can::Socket>();

	global::ucanopen_client = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), global::can_socket);

	std::string server_name(backend_ucanopen_server);
	if (server_name == "SRM Drive")
	{
		global::srmdrive_server = std::make_shared<srmdrive::Server>("SRM Drive", ucanopen::NodeId(0x01), global::can_socket);
		global::ucanopen_client->register_server(global::srmdrive_server);
		
		auto callbackCreateTpdo1 = [](){ return global::srmdrive_server->controller.make_tpdo1(); };
		auto callbackCreateTpdo2 = [](){ return global::srmdrive_server->controller.make_tpdo2(); };

		global::ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(250), callbackCreateTpdo1);
		global::ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(100), callbackCreateTpdo2);
	}
	else if (server_name == "LaunchPad")
	{
		global::launchpad_server = std::make_shared<launchpad::Server>("LaunchPad", ucanopen::NodeId(0x142), global::can_socket);
		global::ucanopen_client->register_server(global::launchpad_server);

		auto callbackCreateTpdo1 = [](){ return global::launchpad_server->create_client_tpdo1(); };
		auto callbackCreateTpdo2 = [](){ return global::launchpad_server->create_client_tpdo2(); };
		auto callbackCreateTpdo3 = [](){ return global::launchpad_server->create_client_tpdo3(); };
		auto callbackCreateTpdo4 = [](){ return global::launchpad_server->create_client_tpdo4(); };

		global::ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(50), callbackCreateTpdo1);
		global::ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(100), callbackCreateTpdo2);
		global::ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(250), callbackCreateTpdo3);
		global::ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(1000), callbackCreateTpdo4);
	}
	else if (server_name == "BMS Main")
	{
		global::bmsmain_server = std::make_shared<bmsmain::Server>("BMS Main", ucanopen::NodeId(0x20), global::can_socket);
		global::ucanopen_client->register_server(global::bmsmain_server);
	}

	backend_ucanopen_server_config_category = global::ucanopen_client->server(server_name)->config_service.config_category.data();

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

	std::future<void> futureExit = signal_exit_main.get_future();
	thread_main = std::thread(backend_main_loop, std::move(futureExit));
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

	signal_exit_main.set_value();
	thread_main.join();
}


