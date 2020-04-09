#include "Network.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

std::tuple<long double, std::string> Network::getNetworkSpeed()
{
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned int i = 0;

    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    ULONG family = AF_UNSPEC;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS* pDnServer = NULL;
    IP_ADAPTER_PREFIX* pPrefix = NULL;

    outBufLen = WORKING_BUFFER_SIZE;

    do {

        pAddresses = (IP_ADAPTER_ADDRESSES*)MALLOC(outBufLen);
        if (pAddresses == NULL) {
            printf
            ("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            exit(1);
        }

        dwRetVal =
            GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            FREE(pAddresses);
            pAddresses = NULL;
        }
        else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal == NO_ERROR) {
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            std::wstring friendlyName(pCurrAddresses->FriendlyName);
            if (friendlyName.find(L"Wi-Fi") != -1 || friendlyName.find(L"Wireless") != -1 
                || friendlyName.find(L"802.11") != -1 || friendlyName.find(L"WiFi") != -1)
            {
                if (((double)pCurrAddresses->TransmitLinkSpeed * 0.000001) > 100000.0 || ((double)pCurrAddresses->TransmitLinkSpeed * 0.000001) == NULL)
                    this->speed = NULL;
                else {
                    this->speed = (double)pCurrAddresses->TransmitLinkSpeed * 0.000001;
                }
                std::wstring network_adapter_description(pCurrAddresses->Description);

                using convert_type = std::codecvt_utf8<wchar_t>;
                std::wstring_convert<convert_type, wchar_t> converter;
                this->adapter_name = converter.to_bytes(network_adapter_description);
            }
            else if (friendlyName.find(L"Ethernet") != -1 || friendlyName.find(L"ethernet") != -1 || friendlyName.find(L"Intel") != -1 || friendlyName.find(L"Realtek") != -1)
            {
                if (((double)pCurrAddresses->TransmitLinkSpeed * 0.000001) > 100000.0 || ((double)pCurrAddresses->TransmitLinkSpeed * 0.000001) == NULL)
                    this->speed = NULL;
                else {
                    this->speed = (double)pCurrAddresses->TransmitLinkSpeed * 0.000001;
                }
                std::wstring network_adapter_description(pCurrAddresses->Description);

                using convert_type = std::codecvt_utf8<wchar_t>;
                std::wstring_convert<convert_type, wchar_t> converter;
                this->adapter_name = converter.to_bytes(network_adapter_description);
            }
            else if (friendlyName.empty())
            {
                this->speed = NULL;
                this->adapter_name = std::string();
            }

            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    else {
        printf("Call to GetAdaptersAddresses failed with error: %d\n",
            dwRetVal);
        if (dwRetVal == ERROR_NO_DATA)
            printf("\tNo addresses were found for the requested parameters\n");
        else {

            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL)) {
                printf("\tError: %s", lpMsgBuf);
                LocalFree(lpMsgBuf);
                if (pAddresses)
                    FREE(pAddresses);
                exit(1);
            }
        }
    }

    if (pAddresses) {
        FREE(pAddresses);
    }

    return { this->speed, this->adapter_name };
}
