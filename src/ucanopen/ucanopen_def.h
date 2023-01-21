#pragma once


#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <charconv>
#include <array>
#include <ios>
#include <map>
#include <cmath>

#include <linux/can.h>


namespace ucanopen {


using can_payload = std::array<uint8_t, 8>;


template <typename T>
inline can_payload to_payload(T message)
{
	static_assert(sizeof(T) <= 8);
	can_payload payload{};
	memcpy(payload.data(), &message, sizeof(T));
	return payload;
}


template <typename T>
inline T from_payload(can_payload payload)
{
	static_assert(sizeof(T) <= 8);
	T message;
	memcpy(&message, payload.data(), sizeof(T));
	return message;
}


class NodeId
{
private:
	unsigned int _value;
public:
	explicit NodeId(unsigned int value) : _value(value) {}
	unsigned int value() const { return _value; }
	bool is_valid() const { return (_value >= 1) && (_value <= 127); }
};


inline bool operator==(const NodeId& lhs, const NodeId& rhs) { return lhs.value() == rhs.value(); }


enum class NmtState
{
	initialization = 0x00,
	stopped = 0x04,
	operational = 0x05,
	pre_operational = 0x7F
};


enum class CobType
{
	nmt,
	sync,
	emcy,
	time,
	tpdo1,
	rpdo1,
	tpdo2,
	rpdo2,
	tpdo3,
	rpdo3,
	tpdo4,
	rpdo4,
	tsdo,
	rsdo,
	heartbeat
};


constexpr size_t cob_type_count = 15;


constexpr std::array<canid_t, cob_type_count> cob_function_codes = {
	0x000,	// NMT
	0x080,	// SYNC
	0x080,	// EMCY
	0x100,	// TIME
	0x180,	// TPDO1
	0x200,	// RPDO1
	0x280,	// TPDO2
	0x300,	// RPDO2
	0x380,	// TPDO3
	0x400,	// RPDO3
	0x480,	// TPDO4
	0x500,	// RPDO4
	0x580,	// TSDO
	0x600,	// RSDO
	0x700	// HEARTBEAT
};


inline canid_t calculate_cob_id(CobType cob_type, NodeId node_id)
{
	if ((cob_type == CobType::nmt) || (cob_type == CobType::sync) || (cob_type == CobType::time))
	{
		return cob_function_codes[static_cast<size_t>(cob_type)];
	}
	return cob_function_codes[static_cast<size_t>(cob_type)] + node_id.value();
}


constexpr std::array<unsigned int, cob_type_count> cob_sizes = {
	2,	// NMT
	0,	// SYNC
	2,	// EMCY
	6,	// TIME
	8,	// TPDO1
	8,	// RPDO1
	8,	// TPDO2
	8,	// RPDO2
	8,	// TPDO3
	8,	// RPDO3
	8,	// TPDO4
	8,	// RPDO4
	8,	// TSDO
	8,	// RSDO
	1	// HEARTBEAT
};


enum class TpdoType
{
	tpdo1,
	tpdo2,
	tpdo3,
	tpdo4,
};


inline CobType to_cob_type(TpdoType tpdo_type)
{
	return static_cast<CobType>(
		static_cast<unsigned int>(CobType::tpdo1) + 2 * static_cast<unsigned int>(tpdo_type)
	);
}


enum class RpdoType
{
	rpdo1,
	rpdo2,
	rpdo3,
	rpdo4,	
};


inline CobType to_cob_type(RpdoType rpdo_type)
{
	return static_cast<CobType>(
		static_cast<unsigned int>(CobType::rpdo1) + 2 * static_cast<unsigned int>(rpdo_type)
	);
}


inline RpdoType opposite_pdo(TpdoType type)
{
	return static_cast<RpdoType>(type);
}


inline TpdoType opposite_pdo(RpdoType type)
{
	return static_cast<TpdoType>(type);
}


enum ODEntryDataType
{
	OD_BOOL,
	OD_INT16,
	OD_INT32,
	OD_UINT16,
	OD_UINT32,
	OD_FLOAT32,
	OD_ENUM16,
	OD_EXEC,
	OD_STRING_4CHARS
};


enum ODEntryAccessPermission
{
	OD_ACCESS_RW,
	OD_ACCESS_RO,
	OD_ACCESS_WO,
};


class CobSdoData
{
private:
	uint32_t _data = 0;
public:
	CobSdoData() = default;
	CobSdoData(bool val) { memcpy(&_data, &val, sizeof(val)); }
	CobSdoData(int16_t val) { memcpy(&_data, &val, sizeof(val)); }
	CobSdoData(int32_t val) { memcpy(&_data, &val, sizeof(_data)); }
	CobSdoData(uint16_t val) { memcpy(&_data, &val, sizeof(val)); }
	CobSdoData(uint32_t val) { memcpy(&_data, &val, sizeof(_data)); }
	CobSdoData(float val) { memcpy(&_data, &val, sizeof(_data)); }

	bool bool32() const
	{
		return _data;
	}

	int16_t i16() const
	{
		int16_t val;
		memcpy(&val, &_data, sizeof(int16_t));
		return val;
	}

	int32_t i32() const
	{
		int32_t val;
		memcpy(&val, &_data, sizeof(int32_t));
		return val;
	}

	uint16_t u16() const
	{ 
		uint16_t val;
		memcpy(&val, &_data, sizeof(uint16_t));
		return val;
	}

	uint32_t u32() const
	{ 
		uint32_t val;
		memcpy(&val, &_data, sizeof(uint32_t));
		return val;
	}
	
	float f32() const
	{ 
		float val;
		memcpy(&val, &_data, sizeof(float));
		return val;
	}

	std::string to_string(ODEntryDataType type) const
	{
		switch (type)
		{
		case ucanopen::OD_BOOL:
			return bool32() ? "true" : "false";
		case ucanopen::OD_INT16:
			return std::to_string(i16());
		case ucanopen::OD_INT32:
			return std::to_string(i32());
		case ucanopen::OD_UINT16:
			return std::to_string(u16());
		case ucanopen::OD_UINT32:
			return std::to_string(u32());
		case ucanopen::OD_FLOAT32:
			{
				std::chars_format format = (fabsf(f32()) >= 0.01) ? std::chars_format::fixed : std::chars_format::general;
				std::array<char, 16> buf;
				if (auto [ptr, ec] = std::to_chars(buf.begin(), buf.end(), f32(), format, 2);
						ec == std::errc())
				{
					return std::string(buf.begin(), ptr);
				}
				else
				{
					return std::make_error_code(ec).message();
				}
			}
		case ucanopen::OD_ENUM16:
			return std::to_string(u16());
		case ucanopen::OD_EXEC:
			return std::string();
		case ucanopen::OD_STRING_4CHARS:
			{
				uint32_t strRaw = u32();
				char cstr[5];
				memcpy(cstr, &strRaw, 4);
				cstr[4] = '\0';
				return std::string(cstr);
			}
		}
		return std::string();
	}
};


struct CobSdo
{
	uint32_t data_size_indicated : 1;
	uint32_t expedited_transfer : 1;
	uint32_t data_empty_bytes : 2;
	uint32_t _reserved : 1;
	uint32_t cs : 3;
	uint32_t index : 16;
	uint32_t subindex : 8;
	CobSdoData data = {};

	CobSdo() = default;

	CobSdo(can_payload payload)
	{
		memcpy(this, payload.data(), sizeof(CobSdo));
	}

	CobSdo(const uint8_t* data)
	{
		memcpy(this, data, sizeof(CobSdo));
	}

	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobSdo));
		return data;
	}

	can_payload to_payload() const
	{
		can_payload payload;
		memcpy(payload.data(), this, sizeof(CobSdo));
		return payload;
	}
};


namespace cs_codes {
const uint32_t sdo_ccs_write = 1;
const uint32_t sdo_scs_write = 3;
const uint32_t sdo_ccs_read = 2;
const uint32_t sdo_scs_read = 2;
}


enum class SdoType
{
	response_to_read,
	response_to_write,
	response_to_exec,
};


struct ODEntryKey
{
	unsigned int index;
	unsigned int subindex;
};


inline bool operator<(const ODEntryKey& lhs, const ODEntryKey& rhs)
{
	return (lhs.index < rhs.index) || ((lhs.index == rhs.index) && (lhs.subindex < rhs.subindex));
}


struct ODEntryValue
{
	std::string_view category;
	std::string_view subcategory;
	std::string_view name;
	std::string_view unit;
	ODEntryDataType data_type;
	ODEntryAccessPermission access_permission;

	bool hasReadPermission() const { return (access_permission == OD_ACCESS_RW) || (access_permission == OD_ACCESS_RO); }
	bool hasWritePermission() const { return (access_permission == OD_ACCESS_RW) || (access_permission == OD_ACCESS_WO); }
};


struct ODEntryValueAux
{
	std::string_view category;
	std::string_view subcategory;
	std::string_view name;
};


inline bool operator<(const ODEntryValueAux& lhs, const ODEntryValueAux& rhs)
{
	return (lhs.category < rhs.category) 
			|| ((lhs.category == rhs.category) && (lhs.subcategory < rhs.subcategory))
			|| ((lhs.category == rhs.category) && (lhs.subcategory == rhs.subcategory) && (lhs.name < rhs.name));
}


using ObjectDictionary = std::map<ODEntryKey, ODEntryValue>;
using ObjectDictionaryAux = std::map<ODEntryValueAux, std::map<ODEntryKey, ODEntryValue>::const_iterator>;


struct ObjectDictionaryConfig
{
	std::string_view watch_category;
	std::string_view watch_subcategory;
	std::string_view config_category;
};


inline can_frame create_frame(CobType cob_type, NodeId node_id, can_payload payload)
{
	can_frame frame;
	frame.can_id = calculate_cob_id(cob_type, node_id);
	frame.len = cob_sizes[static_cast<size_t>(cob_type)];
	std::copy(payload.begin(), std::next(payload.begin(), frame.len), frame.data);
	return frame;
}


inline can_frame create_frame(canid_t id, unsigned char len, can_payload payload)
{
	can_frame frame;
	frame.can_id = id;
	frame.len = len;
	std::copy(payload.begin(), std::next(payload.begin(), frame.len), frame.data);
	return frame;	
}


enum class ODTaskStatus
{
	success = 0,
	fail = 1,
	in_progress = 2,
	started = 3
};


enum class ODRequestStatus
{
	success = 0,
	fail = 1,
	no_access = 2
};

} // namespace ucanopen

