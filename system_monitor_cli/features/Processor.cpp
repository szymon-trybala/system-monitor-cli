#include "Processor.h"

#include <cmath>
#include <intrin.h>
#include <iostream>
#include <windows.h>
#include <malloc.h>    
#include <stdio.h>
#include <tchar.h>

typedef BOOL(WINAPI* LPFN_GLPI)(
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
    PDWORD);

std::tuple<std::string, std::string, int, int> Processor::getCpuInfo()
{
    // CPU NAME
    int cpuInfo[4] = { -1 };
    char CPUBrandString[0x40];

    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    __cpuid(cpuInfo, 0x80000002);
    memcpy(CPUBrandString, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000003);
    memcpy(CPUBrandString + 16, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000004);
    memcpy(CPUBrandString + 32, cpuInfo, sizeof(cpuInfo));

    std::string cpu_name(CPUBrandString);

    // CPU CORES
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto architecture_raw = sysinfo.wProcessorArchitecture;
    std::string architecture; 
    switch (architecture_raw) 
    {
    case 5:
        architecture = "ARM";
        break;
    case 9:
        architecture = "x64";
        break;

    case 12:
        architecture = "ARM64";
        break;
    case 6:
        architecture = "IA64";
        break;
    case 0:
        architecture = "x86";
        break;
    default:
        architecture = "Unknown";
        break;
    }
    auto threads = getCpuCores();
    return { cpu_name, architecture, std::get<0>(threads), std::get<1>(threads) };
}


std::tuple<int, int> Processor::getCpuCores()
{
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    DWORD returnLength = 0;
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;

    glpi = (LPFN_GLPI)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),
        "GetLogicalProcessorInformation");
    if (NULL == glpi)
    {
        // _tprintf(TEXT("\nGetLogicalProcessorInformation is not supported.\n"));
        return std::make_tuple(1, 0);
    }

    while (!done)
    {
        DWORD rc = glpi(buffer, &returnLength);

        if (FALSE == rc)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)
                    free(buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
                    returnLength);

                if (NULL == buffer)
                {
                    // _tprintf(TEXT("\nError: Allocation failure\n"));
                    return std::make_tuple(2, 0);
                }
            }
            else
            {
                // _tprintf(TEXT("\nError %d\n"), GetLastError());
                return std::make_tuple(3, 0);
            }
        }
        else
        {
            done = TRUE;
        }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
        switch (ptr->Relationship)
        {
        case RelationNumaNode:
            // Non-NUMA systems report a single record of this type.
            numaNodeCount++;
            break;

        case RelationProcessorCore:
            processorCoreCount++;

            // A hyperthreaded core supplies more than one logical processor.
            logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
            break;

        case RelationCache:
            // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
            Cache = &ptr->Cache;
            if (Cache->Level == 1)
            {
                processorL1CacheCount++;
            }
            else if (Cache->Level == 2)
            {
                processorL2CacheCount++;
            }
            else if (Cache->Level == 3)
            {
                processorL3CacheCount++;
            }
            break;

        case RelationProcessorPackage:
            // Logical processors share a physical package.
            processorPackageCount++;
            break;

        default:
            // _tprintf(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }

    //_tprintf(TEXT("\nGetLogicalProcessorInformation results:\n"));
    //_tprintf(TEXT("Number of NUMA nodes: %d\n"),
    //    numaNodeCount);
    //_tprintf(TEXT("Number of physical processor packages: %d\n"),
    //    processorPackageCount);
    //_tprintf(TEXT("Number of processor cores: %d\n"),
    //    processorCoreCount);
    //_tprintf(TEXT("Number of logical processors: %d\n"),
    //    logicalProcessorCount);
    //_tprintf(TEXT("Number of processor L1/L2/L3 caches: %d/%d/%d\n"),
    //    processorL1CacheCount,
    //    processorL2CacheCount,
    //    processorL3CacheCount);

    free(buffer);

    return { processorCoreCount, logicalProcessorCount };
}

DWORD Processor::CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
    }

    return bitSetCount;
}

float Processor::CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;

    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

    _previousTotalTicks = totalTicks;
    _previousIdleTicks = idleTicks;
    return std::round(ret * 100);
}

float Processor::getCpuLoad()
{
	FILETIME idleTime, kernelTime, userTime;
	return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), 
		FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}
