#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 


namespace ucanopen {

class ServerHeartbeatService;
class ServerTpdoService;
class ServerRpdoService;


namespace impl {

class Server
{
	friend class ucanopen::ServerHeartbeatService;
	friend class ucanopen::ServerTpdoService;
	friend class ucanopen::ServerRpdoService;
protected:
	std::string _name = "unnamed";
	NodeId _node_id;
	std::shared_ptr<can::Socket> _socket;

	const ObjectDictionary& _dictionary;
	ObjectDictionaryAux _dictionary_aux;

	NmtState _nmtState = NmtState::stopped;
protected:
	virtual void _handle_tsdo(SdoType, ObjectDictionary::const_iterator, CobSdoData) = 0;	
public:
	Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary);

	std::string_view name() const { return _name; }
	NodeId node_id() const { return _node_id; }
	NmtState nmt_state() const { return _nmtState; }

	ODRequestStatus read(std::string_view category, std::string_view subcategory, std::string_view name);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData sdo_data);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);
	ODRequestStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);
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

	ODRequestStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ObjectDictionary::const_iterator& entry_iter, check_read_perm);

	ODRequestStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ObjectDictionary::const_iterator& entry_iter, check_write_perm);
	
	ODRequestStatus _find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
					ObjectDictionary::const_iterator& entry_iter, check_exec_perm);
};

} // namespace impl

} // namespace ucanopen

