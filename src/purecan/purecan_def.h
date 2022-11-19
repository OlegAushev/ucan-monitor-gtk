/**
 * @file purecan_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstdint>
#include <array>
#include <vector>


namespace purecan {


using can_payload = std::array<uint8_t, 8>;
using can_payload_va = std::vector<uint8_t>;


}


