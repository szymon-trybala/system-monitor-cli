#include "System.h"
#include "../Utilities.cpp"


std::tuple<long long, unsigned short, unsigned short, unsigned short> System::getUpTime()
{
	this->ticks = GetTickCount64();

	unsigned int one_hour = 60 * 60 * 1000;
	long long hours = ticks / one_hour;
	unsigned short minutes = (ticks - (hours * one_hour)) / (one_hour / 60);
	unsigned short seconds = (ticks - (hours * one_hour) - (minutes * (one_hour / 60))) / 1000;
	unsigned short miliseconds = ticks - (hours * one_hour) - (minutes * (one_hour / 60)) - (seconds * 1000);

	return { hours, minutes, seconds, miliseconds };
}

std::string System::getComputerName()
{
	COMPUTER_NAME_FORMAT format = ComputerNameNetBIOS;
	TCHAR buffer[256] = TEXT("");
	DWORD dwSize = sizeof(buffer);

	GetComputerNameEx(format, buffer, &dwSize);
	computer_name = CW2A(buffer);
	return computer_name;
}


std::string System::getSystemVersion()
{
    DWORD dwSize = 0;
    BYTE* pbVersionInfo = NULL;
    VS_FIXEDFILEINFO* pFileInfo = NULL;
    UINT puLenFileInfo = 0;
    TCHAR pszPath[MAX_PATH];
    DWORD dwMajor, dwMinor;
    BOOL Is64 = FALSE;

    GetSystemDirectory(pszPath, sizeof(pszPath));

    PathAppend(pszPath, _T("kernel32.dll"));

    dwSize = GetFileVersionInfoSize(pszPath, NULL);

    if (dwSize != 0)
    {
        pbVersionInfo = new BYTE[dwSize];

        if (GetFileVersionInfo(pszPath, 0, dwSize, pbVersionInfo))
        {
            if (VerQueryValue(pbVersionInfo, _T("\\"), (LPVOID*)&pFileInfo, &puLenFileInfo))
            {


                #ifdef ENVIRONMENT64
                dwMajor = pFileInfo->dwProductVersionMS >> 16 & 0xff;
                dwMinor = pFileInfo->dwProductVersionMS >> 0 & 0xff;
                #endif

                #ifdef ENVIRONMENT32
                dwMajor = pFileInfo->dwProductVersionMS;
                dwMinor = pFileInfo->dwProductVersionMS;
                #endif


                std::string relaseId = getValFromReg(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"ReleaseId");
                std::string build = getValFromReg(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");

                if (dwMajor == 5)
                {
                    if (dwMinor == 0) {
                        return "Windows 2000 5.0." + relaseId + " " + build;
                    }
                    else if (dwMinor == 1) {
                        return "Windows XP 5.1." + relaseId + " " + build;
                    }
                }
                else if (dwMajor == 6)
                {
                    if (dwMinor == 0) {
                        return "Windows Vista 6.0" + relaseId + " " + build;
                    }
                    else if (dwMinor == 1) {
                        return "Windows 7 6.1" + relaseId + " " + build;
                    }
                    else if (dwMinor == 2) {
                        return "Windows 8 6.2" + relaseId + " " + build;
                    }
                    else if (dwMinor == 3) {
                        return "Windows 8.1 6.3" + relaseId + " " + build;
                    }
                }
                else if (dwMajor == 10)
                {
                    if (dwMinor == 0) {
                        return  "Windows 10.0." + relaseId + " " + build;
                    }
                }
            }
        }
    }
}
