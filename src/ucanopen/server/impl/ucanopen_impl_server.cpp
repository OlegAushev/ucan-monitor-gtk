#include "ucanopen_impl_server.h"


namespace ucanopen {

impl::Server::Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket, const ObjectDictionary& dictionary)
	: _name(name)
	, _node_id(node_id)
	, _socket(socket)
	, _dictionary(dictionary)
{
	for (const auto& [key, entry] : _dictionary.entries)
	{
		// create aux dictionary for faster search by {category, subcategory, name}
		_dictionary_aux.insert({
				{entry.category, entry.subcategory, entry.name},
				_dictionary.entries.find(key)});
	}
}


ODAccessStatus impl::Server::read(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ODEntryIter entry_iter;
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
	if (entry_iter->second.category != _dictionary.config.watch_category)
	{
		Log() << "[ucanopen/read] Read " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " request sent...\n";
	}
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, ExpeditedSdoData sdo_data)
{
	ODEntryIter entry_iter;
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

	Log() << "[ucanopen/write] Write " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
			<< " = " << sdo_data.to_string(entry_iter->second.data_type) << " request sent...\n";
	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
{
	ODEntryIter entry_iter;
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
				return ODAccessStatus::invalid_value;
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
			return ODAccessStatus::invalid_value;
	}

	ExpeditedSdo message{};
	message.cs = sdo_cs_codes::client_init_write;
	message.expedited_transfer = 1;
	message.data_size_indicated = 1;
	message.data_empty_bytes = 0;
	message.index = entry_iter->first.index;
	message.subindex = entry_iter->first.subindex;
	message.data = sdo_data;

	Log() << "[ucanopen/write] Write " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
			<< " = " << value << " request sent...\n";
	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::exec(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ODEntryIter entry_iter;
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

	Log() << "[ucanopen/exec] Execute " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " request sent...\n";
	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ODEntryIter& ret_entry_iter, check_read_perm)
{
	ret_entry_iter = _find_od_entry(category, subcategory, name);
	if (ret_entry_iter == _dictionary.entries.end())
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry.\n";
		return ODAccessStatus::not_found;
	}
	else if (ret_entry_iter->second.has_read_permission() == false)
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - access denied.\n";
		return ODAccessStatus::access_denied;
	}
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ODEntryIter& ret_entry_iter, check_write_perm)
{
	ret_entry_iter = _find_od_entry(category, subcategory, name);
	if (ret_entry_iter == _dictionary.entries.end())
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry.\n";
		return ODAccessStatus::not_found;;
	}
	else if (ret_entry_iter->second.has_write_permission() == false)
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - access denied.\n";
		return ODAccessStatus::access_denied;
	}
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::_find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ODEntryIter& ret_entry_iter, check_exec_perm)
{
	ret_entry_iter = _find_od_entry(category, subcategory, name);
	if (ret_entry_iter == _dictionary.entries.end())
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry.\n";
		return ODAccessStatus::not_found;
	}
	else if (ret_entry_iter->second.data_type != ODEntryDataType::OD_EXEC)
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - not executable OD entry.\n";
		return ODAccessStatus::access_denied;
	}
	return ODAccessStatus::success;
}

} // namespace ucanopen

