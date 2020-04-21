#pragma once

#include "windows.h"
#include <vector>
#include <string>

class Storage 
{
public:
	std::vector<UINT64> getDiskSizeInfo(std::string path = std::string());
	std::string getDriveType(std::string path = "");
	std::vector<std::string> getMountedPartitions();

private:
	BOOL driveTypeResult;
	LPCSTR driveTypeDefaultPath = NULL;
	UINT64 driveTypeFreeBytesAvaliableToCaller, driveTypeTotalBytes, driveTypeTotalFreeBytes;
};
