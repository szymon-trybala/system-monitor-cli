#pragma once

#include <tuple>
#include <string>
#include <vector>

class Display 
{
public:
	std::tuple<unsigned short, unsigned short>get_display_resolution();
	std::vector<std::string>get_display_info();

private:
	std::string get_main_monitor_name();
};