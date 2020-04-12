#include "features/System.h";
#include <atlstr.h>

inline std::string getValFromReg(const std::wstring &key, const std::wstring  &val) {

    DWORD keyType = 0;
    DWORD dataSize = 0;
    const DWORD flags = RRF_RT_REG_SZ; // Only read strings (REG_SZ)

    LONG result = ::RegGetValue(
        HKEY_LOCAL_MACHINE,
        key.c_str(),
        val.c_str(),
        flags,
        &keyType,
        nullptr, // pvData == nullptr ? Request buffer size
        &dataSize);

    CString text;
    const DWORD bufferLength = dataSize / sizeof(WCHAR); // length in WCHAR's
    WCHAR* const textBuffer = text.GetBuffer(bufferLength);

    result = ::RegGetValue(
        HKEY_LOCAL_MACHINE,
        key.c_str(),
        val.c_str(),
        flags,
        nullptr,
        textBuffer, // Write string in this destination buffer
        &dataSize);

    const DWORD actualStringLength = dataSize / sizeof(WCHAR);

    // -1 to exclude the terminating NUL
    text.ReleaseBufferSetLength(actualStringLength - 1);

    CT2CA pszConvertedAnsiString(text);
    // construct a std::string using the LPCSTR input
    return std::string(pszConvertedAnsiString);
}
