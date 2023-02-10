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


ODAccessStatus impl::Server::read(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_read_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	ExpeditedSdo message{};
	message.cs = sdo_cs_codes::client_init_read;
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, ExpeditedSdoData sdo_data)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_write_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	ExpeditedSdo message{};
	message.cs = sdo_cs_codes::client_init_write;
	message.expedited_transfer = 1;
	message.data_size_indicated = 1;
	message.data_empty_bytes = 0;
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;
	message.data = sdo_data;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_write_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	ExpeditedSdoData sdo_data;

	switch (entry_iter->second.data_type)
	{
		case OD_BOOL:
			if (value == "TRUE" || value == "true" || value == "ON" || value == "on" || value == "1")
				sdo_data = ExpeditedSdoData(true);
			else if (value == "FALSE" || value == "false" || value == "OFF" || value == "off" || value == "0")
				sdo_data = ExpeditedSdoData(true);
			else
				return ODAccessStatus::fail;
			break;
		case OD_INT16:
			sdo_data = ExpeditedSdoData(int16_t(std::stoi(value)));
			break;
		case OD_INT32:
			sdo_data = ExpeditedSdoData(int32_t(std::stoi(value)));
			break;
		case OD_UINT16:
			sdo_data = ExpeditedSdoData(uint16_t(std::stoul(value)));
			break;
		case OD_UINT32:
			sdo_data = ExpeditedSdoData(uint32_t(std::stoul(value)));
			break;
		case OD_FLOAT32:
			sdo_data = ExpeditedSdoData(float(std::stof(value)));
			break;
		case OD_ENUM16:
			sdo_data = ExpeditedSdoData(uint16_t(std::stoi(value)));
			break;
		default:
			return ODAccessStatus::fail;
	}

	ExpeditedSdo message{};
	message.cs = sdo_cs_codes::client_init_write;
	message.expedited_transfer = 1;
	message.data_size_indicated = 1;
	message.data_empty_bytes = 0;
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;
	message.data = sdo_data;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::exec(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ObjectDictionary::const_iterator entry_iter;
	auto status = _find_od_entry(category, subcategory, name, entry_iter, check_exec_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	ExpeditedSdo message{};
	if (entry_iter->second.has_read_permission())
	{
		message.cs = sdo_cs_codes::client_init_read;
	}
	else if (entry_iter->second.has_write_permission())
	{
		message.cs = sdo_cs_codes::client_init_write;
		message.expedited_transfer = 1;
		message.data_size_indicated = 1;
		message.data_empty_bytes = 0;
	}
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ObjectDictionary::const_iterator& entry_iter, check_read_perm)
{
	entry_iter = _find_od_entry(category, subcategory, name);
	if (entry_iter == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODAccessStatus::fail;
	}
	else if (entry_iter->second.has_read_permission() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODAccessStatus::no_access;
	}
	return ODAccessStatus::success;
}





ODAccessStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ObjectDictionary::const_iterator& entry_iter, check_write_perm)
{
	entry_iter = _find_od_entry(category, subcategory, name);
	if (entry_iter == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODAccessStatus::fail;;
	}
	else if (entry_iter->second.has_write_permission() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODAccessStatus::no_access;
	}
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ObjectDictionary::const_iterator& entry_iter, check_exec_perm)
{
	entry_iter = _find_od_entry(category, subcategory, name);
	if (entry_iter == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODAccessStatus::fail;
	}
	else if (entry_iter->second.data_type != ODEntryDataType::OD_EXEC)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - not executable OD entry." << std::endl;
		return ODAccessStatus::no_access;
	}
	return ODAccessStatus::success;
}

} // namespace ucanopen

