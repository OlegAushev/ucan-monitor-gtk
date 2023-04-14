#include "ucanopen_devices/srmdrive/server/srmdrive_server.h"
#include "ucanopen_devices/crd600/server/crd600_server.h"
#include "ucanopen_devices/launchpad/server/launchpad_server.h"
#include "ucanopen_devices/bmsmain/server/bmsmain_server.h"


extern std::shared_ptr<srmdrive::Server> srmdrive_server;
extern std::shared_ptr<crd600::Server> crd600_server;
extern std::shared_ptr<launchpad::Server> launchpad_server;
extern std::shared_ptr<bmsmain::Server> bmsmain_server;


extern "C" {

int ucanopen_devices_get_error_names(const char* server_name, char** retbuf, int str_count, int str_size) {
    if (std::string(server_name) == "SRM-Drive-80") {
        if (srmdrive::error_list.size() > str_count) {
            return 0;
        }

        int i = 0;
        for (auto error : srmdrive::error_list) {
            retbuf[i][0] = '\0';
            strncat(retbuf[i++], error.data(), str_size-1);
        }

        return srmdrive::error_list.size();
    } else if (std::string(server_name) == "CRD600") {
        if (crd600::error_list.size() > str_count) {
            return 0;
        }

        int i = 0;
        for (auto error : crd600::error_list) {
            retbuf[i][0] = '\0';
            strncat(retbuf[i++], error.data(), str_size-1);
        }

        return crd600::error_list.size();
    } else if (std::string(server_name) == "LaunchPad") {
        if (launchpad::error_list.size() > str_count) {
            return 0;
        }

        int i = 0;
        for (auto error : launchpad::error_list) {
            retbuf[i][0] = '\0';
            strncat(retbuf[i++], error.data(), str_size-1);
        }

        return launchpad::error_list.size();
    } else if (std::string(server_name) == "BMS-Main") {
        // TODO
    }

    return 0;
}

int ucanopen_devices_get_warning_names(const char* server_name, char** retbuf, int str_count, int str_size) {
    if (std::string(server_name) == "SRM-Drive-80") {
        if (srmdrive::warning_list.size() > str_count) {
            return 0;
        }

        int i = 0;
        for (auto error : srmdrive::warning_list) {
            retbuf[i][0] = '\0';
            strncat(retbuf[i++], error.data(), str_size-1);
        }

        return srmdrive::warning_list.size();
    } else if (std::string(server_name) == "CRD600") {
        if (crd600::warning_list.size() > str_count) {
            return 0;
        }

        int i = 0;
        for (auto error : crd600::warning_list) {
            retbuf[i][0] = '\0';
            strncat(retbuf[i++], error.data(), str_size-1);
        }

        return crd600::warning_list.size();
    } else if (std::string(server_name) == "LaunchPad") {
        if (launchpad::warning_list.size() > str_count) {
            return 0;
        }

        int i = 0;
        for (auto error : launchpad::warning_list) {
            retbuf[i][0] = '\0';
            strncat(retbuf[i++], error.data(), str_size-1);
        }

        return launchpad::warning_list.size();
    } else if (std::string(server_name) == "BMS-Main") {
        // TODO
    }

    return 0;
}

unsigned int ucanopen_devices_get_error_code(const char* server_name) {
    if (std::string(server_name) == "SRM-Drive-80") {
        return srmdrive_server->errors();
    } else if (std::string(server_name) == "CRD600") {
        return crd600_server->errors();
    } else if (std::string(server_name) == "LaunchPad") {
        return launchpad_server->errors();
    } else if (std::string(server_name) == "BMS-Main") {
        // TODO
    }

    return 0;	
}

unsigned int ucanopen_devices_get_warning_code(const char* server_name) {
    if (std::string(server_name) == "SRM-Drive-80") {
        return srmdrive_server->warnings();
    } else if (std::string(server_name) == "CRD600") {
        return crd600_server->warnings();
    } else if (std::string(server_name) == "LaunchPad") {
        return launchpad_server->warnings();
    } else if (std::string(server_name) == "BMS-Main") {
        // TODO
    }

    return 0;	
}

}

