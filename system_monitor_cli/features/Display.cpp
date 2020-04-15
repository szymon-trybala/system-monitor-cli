#include "Display.h"

#include <Windows.h>
#include <ShellScalingApi.h>

#include <iostream>
#include <locale>
#include <codecvt>

std::tuple<unsigned short, unsigned short> Display::get_display_resolution()
{
	try {
		int vertical = GetSystemMetrics(SM_CXSCREEN);
		int horizontal = GetSystemMetrics(SM_CYSCREEN);
		return std::make_tuple(vertical, horizontal);
	}
	catch(...) {
		std::cerr << "Exception was thrown during getting display resolution, program will exit" << std::endl;
		exit(-1);
	}

}

std::vector<std::string> Display::get_display_info()
{
	try {
		std::vector<std::string> info;

		info.push_back(std::to_string(GetSystemMetrics(SM_CMONITORS)));				// Getting amount of connected monitors
		info.push_back(get_main_monitor_name());									// Getting main display name 

		HDC default_screen = GetDC(NULL);
		info.push_back(std::to_string(GetDeviceCaps(default_screen, VREFRESH)));	// Getting display refresh rate
		info.push_back(std::to_string(GetDeviceCaps(default_screen, BITSPIXEL)));	// Getting adjacent color bits for each pixel.
		info.push_back(std::to_string(GetDeviceCaps(default_screen, COLORRES)));	// Getting color resolution, in bits per pixel

		info.push_back(std::to_string(GetDeviceCaps(default_screen, HORZSIZE)));	// Getting estimated monitor width
		info.push_back(std::to_string(GetDeviceCaps(default_screen, VERTSIZE)));	// Getting estimated monitor height


		// Getting scaling (messed up on newer versions of Windows 10, not reccomended to use)
		HMONITOR primary_monitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
		DEVICE_SCALE_FACTOR scaling_factor;
		HRESULT scaling = GetScaleFactorForMonitor(primary_monitor, &scaling_factor);
		info.push_back(std::to_string((int)scaling_factor));

		return info;
	}
	catch (...) {
		std::cerr << "Exception was thrown during getting monitor's info, program will exit" << std::endl;
		exit(-1);
	}

}

std::string Display::get_main_monitor_name()
{
	try {
		DISPLAY_DEVICE dd;
		dd.cb = sizeof(dd);
		int deviceIndex = 0;
		while (EnumDisplayDevices(0, deviceIndex, &dd, 0))
		{
			std::wstring deviceName = dd.DeviceName;
			int monitorIndex = 0;
			while (EnumDisplayDevices(deviceName.c_str(), monitorIndex, &dd, 0))
			{
				// Converting wstring to string
				std::wstring name = std::wstring(dd.DeviceString);

				using convert_type = std::codecvt_utf8<wchar_t>;
				std::wstring_convert<convert_type, wchar_t> converter;
				std::string name_str = converter.to_bytes(name);

				return name_str;
				// monitorIndex++;
				break;
			}
			// deviceIndex++;
			break;
		}
	}
	catch (...) {
		std::cerr << "Exception was thrown during getting main monitor's name, program will exit" << std::endl;
		exit(-1);
	}

}
