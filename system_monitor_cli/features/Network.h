#pragma once

#include <string>
#include <tuple>
#include <locale>
#include <codecvt>

class Network
{
public:
	std::tuple<long double, std::string> getNetworkSpeed();

private:

	long double speed;
	std::string adapter_name;
};