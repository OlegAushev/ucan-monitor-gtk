#include "impl_server.h"


namespace ucanopen {

impl::Server::Server(std::shared_ptr<can::Socket> socket, NodeId node_id, const std::string& name, const ObjectDictionary& dictionary)
	: _name(name)
	, _node_id(node_id)
	, _socket(socket)
	, _dictionary(dictionary)
{
	for (const auto& [key, object] : _dictionary.entries)
	{
		// create aux dictionary for faster search by {category, subcategory, name}
		_dictionary_aux.insert({
				{object.category, object.subcategory, object.name},
				_dictionary.entries.find(key)});
	}
}


ODAccessStatus impl::Server::read(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ODEntryIter entry;
	auto status = find_od_entry(category, subcategory, name, entry, traits::check_read_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	const auto& [key, object] = *entry;

	ExpeditedSdo message{};
	message.cs = sdo_cs_codes::client_init_read;
	message.index = key.index;
	message.subindex = key.subindex;

	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	if (object.category != _dictionary.config.watch_category)
	{
		Log() << "[ucanopen/read] Read " << object.category << "::" << object.subcategory << "::" << object.name
				<< " request sent...\n";
	}
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, ExpeditedSdoData sdo_data)
{
	ODEntryIter entry;
	auto status = find_od_entry(category, subcategory, name, entry, traits::check_write_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	const auto& [key, object] = *entry;

	ExpeditedSdo message{};
	message.cs = sdo_cs_codes::client_init_write;
	message.expedited_transfer = 1;
	message.data_size_indicated = 1;
	message.data_empty_bytes = 0;
	message.index = key.index;
	message.subindex = key.subindex;
	message.data = sdo_data;

	Log() << "[ucanopen/write] Write " << object.category << "::" << object.subcategory << "::" << object.name
			<< " = " << sdo_data.to_string(object.type) << " request sent...\n";
	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
{
	ODEntryIter entry;
	auto status = find_od_entry(category, subcategory, name, entry, traits::check_write_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	const auto& [key, object] = *entry;
	ExpeditedSdoData sdo_data;

	switch (object.type)
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
	message.index = key.index;
	message.subindex = key.subindex;
	message.data = sdo_data;

	Log() << "[ucanopen/write] Write " << object.category << "::" << object.subcategory << "::" << object.name
			<< " = " << value << " request sent...\n";
	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::exec(std::string_view category, std::string_view subcategory, std::string_view name)
{
	ODEntryIter entry;
	auto status = find_od_entry(category, subcategory, name, entry, traits::check_exec_perm{});
	if (status != ODAccessStatus::success)
	{
		return status;
	}

	const auto& [key, object] = *entry;

	ExpeditedSdo message{};
	message.cs = sdo_cs_codes::client_init_write;
	message.expedited_transfer = 1;
	message.data_size_indicated = 1;
	message.data_empty_bytes = 0;
	message.index = key.index;
	message.subindex = key.subindex;

	Log() << "[ucanopen/exec] Execute " << object.category << "::" << object.subcategory << "::" << object.name
				<< " request sent...\n";
	_socket->send(create_frame(CobType::rsdo, _node_id, message.to_payload()));
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ODEntryIter& ret_entry, traits::check_read_perm)
{
	ret_entry = find_od_entry(category, subcategory, name);
	if (ret_entry == _dictionary.entries.end())
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry.\n";
		return ODAccessStatus::not_found;
	}
	else if (ret_entry->second.has_read_permission() == false)
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - access denied.\n";
		return ODAccessStatus::access_denied;
	}
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ODEntryIter& ret_entry, traits::check_write_perm)
{
	ret_entry = find_od_entry(category, subcategory, name);
	if (ret_entry == _dictionary.entries.end())
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry.\n";
		return ODAccessStatus::not_found;;
	}
	else if (ret_entry->second.has_write_permission() == false)
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - access denied.\n";
		return ODAccessStatus::access_denied;
	}
	return ODAccessStatus::success;
}


ODAccessStatus impl::Server::find_od_entry(std::string_view category, std::string_view subcategory, std::string_view name,
				ODEntryIter& ret_entry, traits::check_exec_perm)
{
	ret_entry = find_od_entry(category, subcategory, name);
	if (ret_entry == _dictionary.entries.end())
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry.\n";
		return ODAccessStatus::not_found;
	}
	else if ((ret_entry->second.type != ODObjectType::OD_EXEC)
			|| (ret_entry->second.has_write_permission() == false))
	{
		Log() << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - not executable OD entry.\n";
		return ODAccessStatus::access_denied;
	}
	return ODAccessStatus::success;
}

} // namespace ucanopen

