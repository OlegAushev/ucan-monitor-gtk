#pragma once


#include <cstdint>
#include <array>
#include <vector>


namespace purecan {

using can_payload = std::array<uint8_t, 8>;
using can_payload_va = std::vector<uint8_t>;


template <typename T>
inline can_payload to_payload(T message)
{
	static_assert(sizeof(T) <= 8);
	can_payload payload{};
	memcpy(payload.data(), &message, sizeof(T));
	return payload;
}


template <typename T>
inline can_payload_va to_payload_va(T message)
{
	static_assert(sizeof(T) <= 8);
	can_payload_va payload(sizeof(T));
	memcpy(payload.data(), &message, sizeof(T));
	return payload;
}

}

