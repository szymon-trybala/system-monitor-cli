#include "System.h"
#pragma warning(disable : 4996)

std::tuple<long long, unsigned short, unsigned short, unsigned short> System::getUpTime()
{
	this->ticks = GetTickCount64();

	unsigned int one_hour = 60 * 60 * 1000;
	long long hours = ticks / one_hour;
	unsigned short minutes = (ticks - (hours * one_hour)) / (one_hour / 60);
	unsigned short seconds = (ticks - (hours * one_hour) - (minutes * (one_hour / 60))) / 1000;
	unsigned short miliseconds = ticks - (hours * one_hour) - (minutes * (one_hour / 60)) - (seconds * 1000);

	return { hours, minutes, seconds, miliseconds };
}

std::string System::getComputerName()
{
	COMPUTER_NAME_FORMAT format = ComputerNameNetBIOS;
	TCHAR buffer[256] = TEXT("");
	DWORD dwSize = sizeof(buffer);

	GetComputerNameEx(format, buffer, &dwSize);
	computer_name = CW2A(buffer);
	return computer_name;
}

std::string System::getSystemVersion()
{
	OSVERSIONINFO info;
	ZeroMemory(&info, sizeof(OSVERSIONINFO));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&info);
	std::string version = std::to_string(info.dwMajorVersion);
	version.append(".").append(std::to_string(info.dwMinorVersion));

	if (IsWindows7OrGreater()) {
		if (version == "6.1") return "Windows 7";
	}
	if (IsWindows8Point1OrGreater()) {
		return "Windows 8";
	}
	else
		return "Windows 10";
}
