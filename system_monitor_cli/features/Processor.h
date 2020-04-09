#pragma once

#include <windows.h>

class Processor
{
public:
	float GetCPULoad();

private:
	static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
	static unsigned long long FileTimeToInt64(const FILETIME& ft) { return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime); }
};
