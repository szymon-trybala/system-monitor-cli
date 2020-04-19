#pragma once

#include <string>
#include <vector>

class Graphics {
public:
	std::vector<std::string > get_gpu_name();
	std::vector<std::string> get_gpu_usage();
};
