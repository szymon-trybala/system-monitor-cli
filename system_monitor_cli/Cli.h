#pragma once

#include <string>

#include "include/cxxopts.hpp"


class Cli {
public:
	std::string parse(int argc, char* argv[]);
};
