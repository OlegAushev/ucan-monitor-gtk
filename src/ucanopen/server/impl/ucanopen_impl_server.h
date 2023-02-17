#pragma once


#include "../../ucanopen_def.h" 
#include <cansocket/cansocket.h>
#include <log/log.h>
#include <list>


namespace ucanopen {

enum class ODAccessStatus
{
	success,
	not_found,
	access_denied,
	invalid_value
};


enum class FrameHandlingStatus
{
	success,
	id_mismatch,
	invalid_format,
	object_not_found,
	irrelevant_frame
};


class ServerHeartbeatService;
class ServerTpdoService;
class ServerRpdoService;
class ServerSdoService;


namespace impl {

class Server
{
	friend class ucanopen::ServerHeartbeatService;
	friend class ucanopen::ServerTpdoService;
	friend class ucanopen::ServerRpdoService;
	friend class ucanopen::ServerSdoService;
protected:
	std::string _name = "unnamed";
	NodeId _node_id;
	std::shared_ptr<can::Socket> _socket;

	const ObjectDictionary& _dictionary;
	ObjectDictionaryAux _dictionary_aux;

	NmtState _nmt_state = NmtState::stopped;
public:
	Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary);
	
	std::string_view name() const { return _name; }
	NodeId node_id() const { return _node_id; }
	NmtState nmt_state() const { return _nmt_state; }
	const ObjectDictionary& dictionary() const { return _dictionary; }

	ODAccessStatus read(std::string_view category, std::string_view subcategory, std::string_view name);
	ODAccessStatus write(std::string_view category, std::string_view subcategory, std::string_view name, ExpeditedSdoData sdo_data);
	ODAccessStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);
	ODAccessStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);
protected:
	virtual void _handle_tsdo(SdoType, ODEntryIter, ExpeditedSdoData) = 0;

	ODEntryIter _find_od_entry(
		std::string_view category,
		std::string_view subcategory,
		std::string_view name)
	{
		auto iter = _dictionary_aux.find({category, subcategory, name});
		if (iter == _dictionary_aux.end())
		{
			return _dictionary.entries.end();
		}
		return iter->second;
	}

	// traits
	struct check_read_perm{};
	struct check_write_perm{};
	struct check_exec_perm{};

	ODAccessStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ODEntryIter& ret_entry_iter, check_read_perm);

	ODAccessStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ODEntryIter& ret_entry_iter, check_write_perm);
	
	ODAccessStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ODEntryIter& ret_entry_iter, check_exec_perm);
};


class FrameHandlingService
{
public:
	virtual FrameHandlingStatus handle_frame(const can_frame& frame) = 0;
};


class SdoSubscriber;


class SdoPublisher
{
public:
	virtual ~SdoPublisher() = default;
	void register_subscriber(SdoSubscriber* subscriber) { _subscriber_list.push_back(subscriber); }
	void unregister_subscriber(SdoSubscriber* subscriber) { _subscriber_list.remove(subscriber); }
protected:
	std::list<SdoSubscriber*> _subscriber_list;
};


class SdoSubscriber
{
private:
	SdoPublisher* _publisher;
public:
	SdoSubscriber(SdoPublisher* publisher)
		: _publisher(publisher)
	{
		_publisher->register_subscriber(this);
	}
	virtual ~SdoSubscriber()
	{
		_publisher->unregister_subscriber(this);
	}
	virtual FrameHandlingStatus handle_sdo(SdoType sdo_type, ODEntryIter entry_iter, ExpeditedSdoData sdo_data) = 0;
	void unsubscribe() { _publisher->unregister_subscriber(this); }
};

} // namespace impl

} // namespace ucanopen

