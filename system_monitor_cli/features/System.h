#pragma once

#pragma comment( lib, "Shlwapi.lib")
#pragma comment( lib, "Version.lib")

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#include "windows.h"
#include <vector>
#include <string>
#include <tuple>


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
