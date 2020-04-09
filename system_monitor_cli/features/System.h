#pragma once

#include "windows.h"
#include <vector>
#include <string>
#include <tuple>
#include <VersionHelpers.h>
#include <atlstr.h>

class System
{
public:
	std::tuple<long long, unsigned short, unsigned short, unsigned short> getUpTime();
	std::string getComputerName();
	std::string getSystemVersion();

private:

	ULONGLONG ticks;
	std::string computer_name;
};
