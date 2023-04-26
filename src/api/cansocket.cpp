#include "cansocket/cansocket.h"


std::shared_ptr<can::Socket> can_socket;


namespace api {
void register_can_socket(std::shared_ptr<can::Socket> can_socket_) {
    can_socket = can_socket_;
}
}


extern "C" {

int cansocket_connect(const char* interface, int bitrate) {
    if (can_socket == nullptr) {
        return -1;
    }

    auto error = can_socket->connect(interface, bitrate);
    return std::to_underlying(error);
}


int cansocket_disconnect() {
    if (can_socket == nullptr) {
        return -1;
    }

    auto error = can_socket->disconnect();
    return std::to_underlying(error);
}

}

