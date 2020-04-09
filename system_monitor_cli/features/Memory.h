#include "windows.h"
#include <vector>
#include <string>

class Mem
{
public:
	std::vector<int> getMemoryInfo();

private:
	MEMORYSTATUSEX statex;
};
