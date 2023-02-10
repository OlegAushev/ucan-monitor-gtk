#pragma once


#include "../../ucanopen_def.h" 
#include <cansocket/cansocket.h>


namespace ucanopen {

enum class ODAccessStatus
{
	success = 0,
	fail = 1,
	no_access = 2
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
protected:
	virtual void _handle_tsdo(SdoType, ObjectDictionary::const_iterator, ExpeditedSdoData) = 0;	
public:
	Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary);

	std::string_view name() const { return _name; }
	NodeId node_id() const { return _node_id; }
	NmtState nmt_state() const { return _nmt_state; }

	ODAccessStatus read(std::string_view category, std::string_view subcategory, std::string_view name);
	ODAccessStatus write(std::string_view category, std::string_view subcategory, std::string_view name, ExpeditedSdoData sdo_data);
	ODAccessStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);
	ODAccessStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);
protected:
	ObjectDictionary::const_iterator _find_od_entry(
		std::string_view category,
		std::string_view subcategory,
		std::string_view name)
	{
		auto iter = _dictionary_aux.find({category, subcategory, name});
		if (iter == _dictionary_aux.end())
		{
			return _dictionary.end();
		}
		return iter->second;
	}

	// traits
	struct check_read_perm{};
	struct check_write_perm{};
	struct check_exec_perm{};

	ODAccessStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ObjectDictionary::const_iterator& entry_iter, check_read_perm);

	ODAccessStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ObjectDictionary::const_iterator& entry_iter, check_write_perm);
	
	ODAccessStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ObjectDictionary::const_iterator& entry_iter, check_exec_perm);
};

} // namespace impl

enum class HandlingStatus
{
	success,
	fail,
	invalid_id
};


namespace impl {

class FrameHandlingService
{
public:
	virtual HandlingStatus handle_frame(const can_frame& frame) = 0;
};

} // namespace impl

} // namespace ucanopen

