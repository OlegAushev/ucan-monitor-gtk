/**
 * @file c_motordrive_dataprocessor.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include <string_view>
#include <array>
#include <charconv>


extern "C" {


///
///
///
void motordrive_data_get_watch(const char* name, char* value)
{
	if (std::string_view(name) == "UPTIME")
	{
		static float counter = 0.4242;
		std::array<char, 16> buf;
		if (auto [ptr, ec] = std::to_chars(buf.begin(), buf.end(), counter, std::chars_format::fixed, 2);
				ec == std::errc())
		{
			std::copy(buf.begin(), ptr, value);
		}
		else
		{
			
		}
		counter += 1.0;
	}
}








}


