#include "Storage.h"

#include <math.h>
#include <string>
#include <iostream>
#include "../include/cxxopts.hpp"



	std::vector<UINT64> Storage::getDiskSizeInfo(std::string path)
	{
		if (path == "") 
		{
			driveTypeResult = GetDiskFreeSpaceExA(
				driveTypeDefaultPath,
				(PULARGE_INTEGER)&driveTypeFreeBytesAvaliableToCaller,
				(PULARGE_INTEGER)&driveTypeTotalBytes,
				(PULARGE_INTEGER)&driveTypeTotalFreeBytes
			);
		}
		else 
		{
			driveTypeResult = GetDiskFreeSpaceExA(
				path.c_str(),
				(PULARGE_INTEGER)&driveTypeFreeBytesAvaliableToCaller,
				(PULARGE_INTEGER)&driveTypeTotalBytes,
				(PULARGE_INTEGER)&driveTypeTotalFreeBytes
			);
		}
		
		if (driveTypeResult == 0)
			return std::vector<UINT64>();

		std::vector<UINT64> vector;
		vector.push_back((driveTypeTotalBytes - driveTypeTotalFreeBytes) / (pow(1024, 2)));
		vector.push_back(driveTypeTotalFreeBytes / (pow(1024, 2)));
		vector.push_back(driveTypeTotalBytes / (pow(1024, 2)));

		return vector;
	}

	std::string Storage::getDriveType(std::string path)
	{
		UINT result;
		if (path == std::string()) 
		{
			result = GetDriveTypeA(NULL);
		}
		else
		{
			result = GetDriveTypeA(path.c_str());
		}

		switch (result) 
		{
		case 0:
			return std::string();
		case 1:
			return std::string();
		case 2:
			return "Dysk zewnêtrzny";
		case 3:
			return "Dysk twardy";
		case 4:
			return "Dysk sieciowy";
		case 5:
			return "Dysk CD";
		case 6:
			return "Dysk RAM";
		default:
			return std::string();
		}
	}

	std::vector<std::string> Storage::getMountedPartitions()
	{
		try {
			std::vector<std::string> arrayOfDrives;
			char* szDrives = new char[MAX_PATH]();
			if (GetLogicalDriveStringsA(MAX_PATH, szDrives));
			for (int i = 0; i < 100; i += 4)
				if (szDrives[i] != (char)0) {
					std::string driveLetter = std::string{ szDrives[i],szDrives[i + 1],szDrives[i + 2] };
					if (getDriveType(driveLetter) != "Dysk CD")
						arrayOfDrives.push_back(driveLetter);
				}

			delete[] szDrives;
			return arrayOfDrives;
		}
		catch (const cxxopts::OptionException& e) {
			std::cerr << "Error while probing partitions: " << e.what() << std::endl;
			exit(1);
		}
	}
