#include "log.h"


extern "C" {

bool log_get_message(char* buf, size_t len) {
    if (Log::_stream.getline(buf, len)) {
        return true;
    }
    else {
        Log::_stream.clear();
        return false;
    }
} 

}

