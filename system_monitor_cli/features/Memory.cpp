#include "Memory.h"

#include <iostream>
using namespace std;
std::vector<int> Mem::getMemoryInfo()
{
	try {
		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);
		std::vector<int> vector;
		vector.push_back(statex.dwMemoryLoad);
		vector.push_back((statex.ullTotalPhys) / 1024 / 1024);
		vector.push_back(statex.ullAvailPhys / 1024 / 1024);
		vector.push_back((statex.ullTotalPhys - statex.ullAvailPhys) / 1024 / 1024);

		return vector;
	}
	catch (...) {
		std::cerr << "Exception was thrown during getting RAM info, program will exit" << std::endl;
		exit(-1);
	}

}
