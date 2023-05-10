#pragma once


#include "../impl/impl_server.h"
#include <vector>
#include <map>
#include <mutex>


namespace ucanopen {

class ServerWatchService : public SdoSubscriber {
private:
    impl::Server& _server;
    bool _is_enabled = false;
    std::chrono::milliseconds _period = std::chrono::milliseconds(1000);
    std::chrono::time_point<std::chrono::steady_clock> _timepoint;
    std::vector<std::string_view> _object_list;
    mutable std::mutex _data_access_mutex;
    std::map<std::string_view, std::string> _data;
public:
    ServerWatchService(impl::Server& server, impl::SdoPublisher& sdo_publisher);

    void send() {
        if (_is_enabled && !_object_list.empty()) {
            auto now = std::chrono::steady_clock::now();
            if (now - _timepoint >= _period) {
                static int i = 0;
                _server.read(_server.dictionary().config.watch_category,
                              _server.dictionary().config.watch_subcategory,
                              _object_list[i]);
                _timepoint = now;
                i = (i + 1) % _object_list.size();
            }
        }
    }

    virtual FrameHandlingStatus handle_sdo(ODEntryIter entry, SdoType sdo_type, ExpeditedSdoData sdo_data) override {
        const auto& [key, object] = *entry;

        if ((object.category == _server.dictionary().config.watch_category) && (sdo_type == SdoType::response_to_read)) {
            if (object.type != OD_ENUM16) {
                std::lock_guard<std::mutex> lock(_data_access_mutex);
                _data[object.name] = sdo_data.to_string(object.type, 2);
            }
            return FrameHandlingStatus::success;
        }
        return FrameHandlingStatus::irrelevant_frame;
    }

    void enable() {
        _is_enabled = true;
        Log() << "Enabled uCANopen server {" << _server.name() << "} watch requests (period = " << _period << ").\n" << LogPrefix::ok;
    }

    void disable() {
        _is_enabled = false;
        Log() << "Disabled uCANopen server {" << _server.name() << "} watch requests.\n" << LogPrefix::ok;
    }

    void set_period(std::chrono::milliseconds period) {
        _period = period;
        Log() << "Set uCANopen server {" << _server.name() << "} watch requests period = " << period << ".\n" << LogPrefix::ok;
    }

    std::vector<std::string_view> object_list() const {
        return _object_list;
    }

    std::string value(std::string_view watch_name) const {
        auto it = _data.find(watch_name);
        if (it == _data.end()) {
            return "n/a";
        }
        return it->second;
    }

    void value(std::string_view watch_name, char* retbuf, int bufsize) const {
        retbuf[0] = '\0';
        auto it = _data.find(watch_name);
        if (it == _data.end()) {
            const char* str = "n/a";
            std::strncat(retbuf, str, bufsize-1);
            return;
        }
        std::lock_guard<std::mutex> lock(_data_access_mutex);
        std::strncat(retbuf, it->second.c_str(), bufsize-1);
    }

    void set_value(std::string_view watch_name, const std::string& val) {
        if (!_data.contains(watch_name)) { return; }
        std::lock_guard<std::mutex> lock(_data_access_mutex);
        _data[watch_name] = val;
    }
};

} // namespace ucanopen

