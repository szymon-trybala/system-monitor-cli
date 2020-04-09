#include "Memory.h"

#include <iostream>
using namespace std;
std::vector<int> Mem::getMemoryInfo()
{
	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);
	std::vector<int> vector;
	vector.push_back(statex.dwMemoryLoad);   
	vector.push_back((statex.ullTotalPhys) / 1024 / 1024); 
	vector.push_back(statex.ullAvailPhys / 1024 / 1024); 
	vector.push_back((statex.ullTotalPhys - statex.ullAvailPhys) / 1024 / 1024);

	return vector;
}
