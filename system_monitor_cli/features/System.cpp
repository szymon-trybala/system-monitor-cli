#include "System.h"
#include "../Utilities.cpp"

#include <iostream>

std::tuple<ULONGLONG, ULONGLONG, ULONGLONG, ULONGLONG> System::getUpTime()
{
    try {
        this->ticks = GetTickCount64();

        ULONGLONG miliseconds = ticks % 1000;
        ticks /= 1000;
        ULONGLONG seconds = ticks % 1000;
        ticks /= 60;
        ULONGLONG minutes = ticks % 60;
        ticks /= 60;
        ULONGLONG hours = ticks;       // When "Fast startup" in Windows settings is on, turning off computer doesn't reset counter!

        return { hours, minutes, seconds, miliseconds };
    }
    catch (...) {
        std::cerr << "Exception was thrown during getting uptime, program will exit" << std::endl;
        exit(-1);
    }

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

                dwMajor = pFileInfo->dwProductVersionMS >> 16 & 0xff;
                dwMinor = pFileInfo->dwProductVersionMS >> 0 & 0xff;
                std::string build = getValFromReg(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");
              

                if (dwMajor == 5)
                {
                    if (dwMinor == 0) {
                        return "Windows 2000 5.0 " + build;
                    }
                    else if (dwMinor == 1) {
                        return "Windows XP 5.1 " + build;
                    }
                }
                else if (dwMajor == 6)
                {
                    if (dwMinor == 0) {
                        return "Windows Vista 6.0 " + build;
                    }
                    else if (dwMinor == 1) {
                        return "Windows 7 6.1 "+ build;
                    }
                    else if (dwMinor == 2) {
                        return "Windows 8 6.2 " + build;
                    }
                    else if (dwMinor == 3) {
                        return "Windows 8.1 6.3 " + build;
                    }
                }
                else if (dwMajor == 10)
                {
                    std::string relaseId = getValFromReg(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"ReleaseId");

                    if (dwMinor == 0) {
                        return  "Windows 10.0." + relaseId + " " + build;
                    }
                }
            }
        }
    }
}
