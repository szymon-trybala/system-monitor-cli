#pragma once

#include <string>
#include <tuple>

#include <windows.h>

class Processor
{
public:
	float getCpuLoad();
	std::tuple<std::string, std::string, int, int> getCpuInfo();

private:
	static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
	static unsigned long long FileTimeToInt64(const FILETIME& ft) { return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime); }
	DWORD CountSetBits(ULONG_PTR bitMask);
	std::tuple<int, int> getCpuCores();
};
