#include <ucanopen/client/ucanopen_client.h>
#include <ucanopen_devices/srmdrive/server/srmdrive_server.h>
#include <ucanopen_devices/launchpad/server/launchpad_server.h>
#include <ucanopen_devices/bmsmain/server/bmsmain_server.h>
#include <gnuplotter/gnuplotter.h>


namespace api {
extern void register_can_socket(std::shared_ptr<can::Socket> can_socket_);
extern void register_ucanopen_client(std::shared_ptr<ucanopen::Client> ucanopen_client_);
extern void register_srmdrive_server(std::shared_ptr<srmdrive::Server> srmdrive_server_);
extern void register_launchpad_server(std::shared_ptr<launchpad::Server> launchpad_server_);
extern void register_bmsmain_server(std::shared_ptr<bmsmain::Server> bmsmain_server_);
}


// used in VALA code
bool backend_is_ready = false;
const char* backend_ucanopen_server;
const char* backend_ucanopen_server_list[4] = {"SRM Drive", "CRD600", "LaunchPad", "BMS Main"};
const char* backend_ucanopen_server_config_category;


namespace {
std::thread thread_main;
std::promise<void> signal_exit_main;
}


extern "C" 
int backend_main_loop(std::future<void> futureExit)
{
	std::cout << "[backend] Main loop thread started. Thread id: " << std::this_thread::get_id() << std::endl;

	auto can_socket = std::make_shared<can::Socket>();

	auto ucanopen_client = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), can_socket);

	std::shared_ptr<srmdrive::Server> srmdrive_server;
	std::shared_ptr<launchpad::Server> launchpad_server;
	std::shared_ptr<bmsmain::Server> bmsmain_server;

	std::string server_name(backend_ucanopen_server);
	if (server_name == "SRM Drive")
	{
		srmdrive_server = std::make_shared<srmdrive::Server>("SRM Drive", ucanopen::NodeId(0x01), can_socket);
		ucanopen_client->register_server(srmdrive_server);
		
		auto callbackCreateTpdo1 = [srmdrive_server](){ return srmdrive_server->controller.make_tpdo1(); };
		auto callbackCreateTpdo2 = [srmdrive_server](){ return srmdrive_server->controller.make_tpdo2(); };

		ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(250), callbackCreateTpdo1);
		ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(100), callbackCreateTpdo2);
	}
	else if (server_name == "LaunchPad")
	{
		launchpad_server = std::make_shared<launchpad::Server>("LaunchPad", ucanopen::NodeId(0x142), can_socket);
		ucanopen_client->register_server(launchpad_server);

		auto callbackCreateTpdo1 = [launchpad_server](){ return launchpad_server->create_client_tpdo1(); };
		auto callbackCreateTpdo2 = [launchpad_server](){ return launchpad_server->create_client_tpdo2(); };
		auto callbackCreateTpdo3 = [launchpad_server](){ return launchpad_server->create_client_tpdo3(); };
		auto callbackCreateTpdo4 = [launchpad_server](){ return launchpad_server->create_client_tpdo4(); };

		ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(50), callbackCreateTpdo1);
		ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(100), callbackCreateTpdo2);
		ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(250), callbackCreateTpdo3);
		ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(1000), callbackCreateTpdo4);
	}
	else if (server_name == "BMS Main")
	{
		bmsmain_server = std::make_shared<bmsmain::Server>("BMS Main", ucanopen::NodeId(0x20), can_socket);
		ucanopen_client->register_server(bmsmain_server);
	}

	backend_ucanopen_server_config_category = ucanopen_client->server(server_name)->config_service.config_category.data();

	api::register_can_socket(can_socket);
	api::register_ucanopen_client(ucanopen_client);
	api::register_srmdrive_server(srmdrive_server);
	api::register_launchpad_server(launchpad_server);
	api::register_bmsmain_server(bmsmain_server);

	std::cout << "[backend] Backend ready." << std::endl;
	backend_is_ready = true;

	while (futureExit.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout)
	{

	}

	std::cout << "[backend] Main loop thread stopped." << std::endl;
	return 0;
}


extern "C"
int backend_main_enter()
{
	std::cout << "[backend] Thread id: " << std::this_thread::get_id() << std::endl;
	std::cout << "[backend] Starting new thread for main loop..." << std::endl;

	std::future<void> futureExit = signal_exit_main.get_future();
	thread_main = std::thread(backend_main_loop, std::move(futureExit));
	return 0;
}


extern "C"
void backend_main_exit()
{
	std::cout << "[backend] Sending signal to main loop thread to stop..." << std::endl;

	signal_exit_main.set_value();
	thread_main.join();
}

