/**
 * @file ucanopen_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <charconv>
#include <array>
#include <ios>
#include <map>

#include <linux/can.h>


namespace ucanopen {


class NodeId
{
private:
	unsigned int m_value;
public:
	explicit NodeId(unsigned int value) : m_value(value) {}
	unsigned int value() const { return m_value; }
};


enum class NmtState
{
	INITIALIZATION = 0x00,
	STOPPED = 0x04,
	OPERATIONAL = 0x05,
	PRE_OPERATIONAL = 0x7F
};


enum class CobType
{
	NMT,
	SYNC,
	EMCY,
	TIME,
	TPDO1,
	RPDO1,
	TPDO2,
	RPDO2,
	TPDO3,
	RPDO3,
	TPDO4,
	RPDO4,
	TSDO,
	RSDO,
	HEARTBEAT
};


const size_t COB_TYPE_COUNT = 15;


constexpr std::array<unsigned int, COB_TYPE_COUNT> cobFunctionCode = {
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


inline unsigned int cobId(CobType cobType, unsigned int nodeId)
{
	if ((cobType == CobType::NMT) || (cobType == CobType::SYNC) || (cobType == CobType::TIME))
	{
		return cobFunctionCode[static_cast<size_t>(cobType)];
	}
	return cobFunctionCode[static_cast<size_t>(cobType)] + nodeId;
}


constexpr std::array<unsigned int, COB_TYPE_COUNT> cobDataLen = {
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


/// TPDO type
enum class TpdoType
{
	TPDO1,
	TPDO2,
	TPDO3,
	TPDO4,
};


inline CobType toCobType(TpdoType tpdoType)
{
	return static_cast<CobType>(
		static_cast<size_t>(CobType::TPDO1) + 2 * static_cast<size_t>(tpdoType)
	);
}


/// RPDO type
enum class RpdoType
{
	RPDO1,
	RPDO2,
	RPDO3,
	RPDO4,	
};


inline CobType toCobType(RpdoType rpdoType)
{
	return static_cast<CobType>(
		static_cast<size_t>(CobType::RPDO1) + 2 * static_cast<size_t>(rpdoType)
	);
}


inline RpdoType oppositePdo(TpdoType type)
{
	return static_cast<RpdoType>(type);
}


inline TpdoType oppositePdo(RpdoType type)
{
	return static_cast<TpdoType>(type);
}


/// OD entry data types
enum ODEntryDataType
{
	OD_BOOL,
	OD_INT16,
	OD_INT32,
	OD_UINT16,
	OD_UINT32,
	OD_FLOAT32,
	OD_ENUM16,
	OD_TASK,
	OD_STRING
};


/// OD entry access right types
enum ODEntryAccessRight
{
	OD_ACCESS_RW,
	OD_ACCESS_RO,
	OD_ACCESS_WO,
};


/**
 * @brief SDO data.
 * 
 */
class CobSdoData
{
private:
	uint32_t m_data = 0;
public:
	CobSdoData() = default;
	CobSdoData(bool val) { memcpy(&m_data, &val, sizeof(val)); }
	CobSdoData(int16_t val) { memcpy(&m_data, &val, sizeof(val)); }
	CobSdoData(int32_t val) { memcpy(&m_data, &val, sizeof(m_data)); }
	CobSdoData(uint16_t val) { memcpy(&m_data, &val, sizeof(val)); }
	CobSdoData(uint32_t val) { memcpy(&m_data, &val, sizeof(m_data)); }
	CobSdoData(float val) { memcpy(&m_data, &val, sizeof(m_data)); }

	bool bool32() const
	{
		return m_data;
	}

	int16_t i16() const
	{
		int16_t val;
		memcpy(&val, &m_data, sizeof(int16_t));
		return val;
	}

	int32_t i32() const
	{
		int32_t val;
		memcpy(&val, &m_data, sizeof(int32_t));
		return val;
	}

	uint16_t u16() const
	{ 
		uint16_t val;
		memcpy(&val, &m_data, sizeof(uint16_t));
		return val;
	}

	uint32_t u32() const
	{ 
		uint32_t val;
		memcpy(&val, &m_data, sizeof(uint32_t));
		return val;
	}
	
	float f32() const
	{ 
		float val;
		memcpy(&val, &m_data, sizeof(float));
		return val;
	}

	std::string toString(ODEntryDataType type) const
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
				std::array<char, 16> buf;
				if (auto [ptr, ec] = std::to_chars(buf.begin(), buf.end(), f32(), std::chars_format::fixed, 2);
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
		case ucanopen::OD_TASK:
			return std::string();
		case ucanopen::OD_STRING:
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


/**
 * @brief SDO message.
 * 
 */
struct CobSdo
{
	uint32_t dataSizeIndicated : 1;
	uint32_t expeditedTransfer : 1;
	uint32_t dataEmptyBytes : 2;
	uint32_t reserved : 1;
	uint32_t cs : 3;
	uint32_t index : 16;
	uint32_t subindex : 8;
	CobSdoData data = {};
	CobSdo() = default;
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobSdo));
		return data;
	}
};


// SDO cs-codes
const uint32_t SDO_CCS_WRITE = 1;
const uint32_t SDO_SCS_WRITE = 3;
const uint32_t SDO_CCS_READ = 2;
const uint32_t SDO_SCS_READ = 2;


// Received SDO types
enum SdoType
{
	RESPONSE_TO_READ,
	RESPONSE_TO_WRITE,
	RESPONSE_TO_TASK,
};


/**
 * @brief OD entry key.
 * 
 */
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
	std::string category;
	std::string subcategory;
	std::string name;
	std::string unit;
	ODEntryDataType dataType;
	ODEntryAccessRight accessRight;

	/**
	 * @brief Checks OD-entry read access.
	 * @param (none)
	 * @return \c true if entry has read access, \c false otherwise.
	 */
	bool hasReadAccess() const
	{
		return (accessRight == OD_ACCESS_RW) || (accessRight == OD_ACCESS_RO);
	}


	/**
	 * @brief Checks OD-entry write access.
	 * @param (none)
	 * @return \c true if entry has write access, \c false otherwise.
	 */
	bool hasWriteAccess() const
	{
		return (accessRight == OD_ACCESS_RW) || (accessRight == OD_ACCESS_WO);
	}
};


struct ODEntryValueAux
{
	std::string category;
	std::string subcategory;
	std::string name;
};


inline bool operator<(const ODEntryValueAux& lhs, const ODEntryValueAux& rhs)
{
	return (lhs.category < rhs.category) 
			|| ((lhs.category == rhs.category) && (lhs.subcategory < rhs.subcategory))
			|| ((lhs.category == rhs.category) && (lhs.subcategory == rhs.subcategory) && (lhs.name < rhs.name));
}


typedef std::map<ODEntryKey, ODEntryValue> ObjectDictionaryType;
typedef std::map<ODEntryValueAux, std::map<ODEntryKey, ODEntryValue>::const_iterator> ObjectDictionaryAuxType;


/**
 * @brief Makes CAN frame.
 * 
 * @param cobType 
 * @param nodeId 
 * @param data 
 * @return CAN frame.
 */
inline can_frame makeFrame(CobType cobType, NodeId nodeId, std::array<uint8_t, 8> data)
{
	can_frame frame;
	frame.can_id = cobId(cobType, nodeId.value());
	frame.len = cobDataLen[static_cast<size_t>(cobType)];
	memcpy(frame.data, &data, frame.len);
	return frame;
}


/// OD_TASK execution status
enum ODTaskStatus
{
	TASK_SUCCESS = 0,
	TASK_FAIL = 1,
	TASK_IN_PROGRESS = 2,
	TASK_STARTED = 3
};


/// OD request status
enum class ODRequestStatus
{
	REQUEST_SUCCESS = 0,
	REQUEST_FAIL = 1,
	REQUEST_NO_ACCESS = 2
};


} // namespace ucanopen


