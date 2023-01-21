#include "ucanopen_impl_server.h"


namespace ucanopen {

impl::Server::Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket, const ObjectDictionary& dictionary)
	: _name(name)
	, _node_id(node_id)
	, _socket(socket)
	, _dictionary(dictionary)
{
	for (const auto& [key, entry] : _dictionary)
	{
		// create aux dictionary for faster search by {category, subcategory, name}
		_dictionary_aux.insert({
				{entry.category, entry.subcategory, entry.name},
				_dictionary.find(key)});
	}
}


ODRequestStatus impl::Server::read(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_read_perm{});
	if (status != ODRequestStatus::success)
	{
		return status;
	}

	CobSdo message{};
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;
	message.cs = cs_codes::sdo_ccs_read;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODRequestStatus::success;
}


ODRequestStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData sdo_data)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_write_perm{});
	if (status != ODRequestStatus::success)
	{
		return status;
	}

	CobSdo message{};
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;
	message.cs = cs_codes::sdo_ccs_write;
	message.data = sdo_data;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODRequestStatus::success;
}


ODRequestStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_write_perm{});
	if (status != ODRequestStatus::success)
	{
		return status;
	}

	CobSdoData sdo_data;

	switch (entry_iter->second.data_type)
	{
	case OD_BOOL:
		if (value == "TRUE" || value == "true" || value == "ON" || value == "on" || value == "1")
			sdo_data = CobSdoData(true);
		else if (value == "FALSE" || value == "false" || value == "OFF" || value == "off" || value == "0")
			sdo_data = CobSdoData(true);
		else
			return ODRequestStatus::fail;
		break;
	case OD_INT16:
		sdo_data = CobSdoData(int16_t(std::stoi(value)));
		break;
	case OD_INT32:
		sdo_data = CobSdoData(int32_t(std::stoi(value)));
		break;
	case OD_UINT16:
		sdo_data = CobSdoData(uint16_t(std::stoul(value)));
		break;
	case OD_UINT32:
		sdo_data = CobSdoData(uint32_t(std::stoul(value)));
		break;
	case OD_FLOAT32:
		sdo_data = CobSdoData(float(std::stof(value)));
		break;
	case OD_ENUM16:
		sdo_data = CobSdoData(uint16_t(std::stoi(value)));
		break;
	default:
		return ODRequestStatus::fail;
	}

	CobSdo message{};
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;
	message.cs = cs_codes::sdo_ccs_write;
	message.data = sdo_data;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODRequestStatus::success;
}


ODRequestStatus impl::Server::exec(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_exec_perm{});
	if (status != ODRequestStatus::success)
	{
		return status;
	}

	CobSdo message{};
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;

	if (entry_iter->second.hasReadPermission())
	{
		message.cs = cs_codes::sdo_ccs_read;
	}
	else if (entry_iter->second.hasWritePermission())
	{
		message.cs = cs_codes::sdo_ccs_write;
	}

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODRequestStatus::success;
}


ODRequestStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ObjectDictionary::const_iterator& entry_iter, check_read_perm)
{
	entry_iter = _find_od_entry(category, subcategory, name);
	if (entry_iter == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::fail;
	}
	else if (entry_iter->second.hasReadPermission() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::no_access;
	}
	return ODRequestStatus::success;
}





ODRequestStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ObjectDictionary::const_iterator& entry_iter, check_write_perm)
{
	entry_iter = _find_od_entry(category, subcategory, name);
	if (entry_iter == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::fail;;
	}
	else if (entry_iter->second.hasWritePermission() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::no_access;
	}
	return ODRequestStatus::success;
}







ODRequestStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ObjectDictionary::const_iterator& entry_iter, check_exec_perm)
{
	entry_iter = _find_od_entry(category, subcategory, name);
	if (entry_iter == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::fail;
	}
	else if (entry_iter->second.data_type != ODEntryDataType::OD_TASK)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - not executable OD entry." << std::endl;
		return ODRequestStatus::no_access;
	}
	return ODRequestStatus::success;
}






} // namespace ucanopen

