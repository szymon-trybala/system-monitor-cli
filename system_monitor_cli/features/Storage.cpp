#include "Storage.h"

#include <math.h>
#include <string>

	std::vector<UINT64> Storage::getDiskSizeInfo(std::string path)
	{
		if (path == std::string()) 
		{
			driveTypeResult = GetDiskFreeSpaceExA(
				driveTypeDefaultPath,
				(PULARGE_INTEGER)&driveTypeFreeBytesAvaliableToCaller,
				(PULARGE_INTEGER)&driveTypeTotalBytes,
				(PULARGE_INTEGER)&driveTyoeTotalFreeBytes
			);
		}
		else 
		{
			driveTypeResult = GetDiskFreeSpaceExA(
				path.c_str(),
				(PULARGE_INTEGER)&driveTypeFreeBytesAvaliableToCaller,
				(PULARGE_INTEGER)&driveTypeTotalBytes,
				(PULARGE_INTEGER)&driveTyoeTotalFreeBytes
			);
		}
		
		if (driveTypeResult == 0)
			return std::vector<UINT64>();

		std::vector<UINT64> vector;
		vector.push_back((driveTypeTotalBytes - driveTyoeTotalFreeBytes) / (pow(1000, 2)));
		vector.push_back(driveTyoeTotalFreeBytes / (pow(1000, 2)));
		vector.push_back(driveTypeTotalBytes / (pow(1000, 2)));

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
			return "Wysuwalny nosnik";
		case 3:
			return "Dysk twardy";
		case 4:
			return "Dysk sieciowy";
		case 5:
			return "Naped CD";
		case 6:
			return "Dysk RAM";
		default:
			return std::string();
		}
	}