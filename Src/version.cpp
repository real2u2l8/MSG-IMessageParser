#include "version.h"
#include <windows.h>
#include <memory>

// 리소스에서 버전 정보를 읽어오는 헬퍼 함수
std::wstring GetVersionInfoString(const std::wstring& subBlock) {
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    
    DWORD dwHandle = 0;
    DWORD dwSize = GetFileVersionInfoSizeW(szPath, &dwHandle);
    if (dwSize == 0) {
        return L"Unknown";
    }
    
    std::unique_ptr<BYTE[]> buffer(new BYTE[dwSize]);
    if (!GetFileVersionInfoW(szPath, dwHandle, dwSize, buffer.get())) {
        return L"Unknown";
    }
    
    LPVOID pvProductName = NULL;
    UINT iProductNameLen = 0;
    if (VerQueryValueW(buffer.get(), subBlock.c_str(), &pvProductName, &iProductNameLen)) {
        return std::wstring(static_cast<wchar_t*>(pvProductName));
    }
    
    return L"Unknown";
}

// 실제 구현부
std::wstring Version::GetFileVersion() {
    return GetVersionInfoString(L"\\StringFileInfo\\041204b0\\FileVersion");
}

std::wstring Version::GetProductVersion() {
    return GetVersionInfoString(L"\\StringFileInfo\\041204b0\\ProductVersion");
}

std::wstring Version::GetProductName() {
    return GetVersionInfoString(L"\\StringFileInfo\\041204b0\\ProductName");
}

std::wstring Version::GetLegalCopyright() {
    return GetVersionInfoString(L"\\StringFileInfo\\041204b0\\LegalCopyright");
}

std::wstring Version::GetInternalName() {
    return GetVersionInfoString(L"\\StringFileInfo\\041204b0\\InternalName");
}

std::wstring Version::GetOriginalFilename() {
    return GetVersionInfoString(L"\\StringFileInfo\\041204b0\\OriginalFilename");
}

void Version::ShowBanner() {
    std::wcout << PROGRAM_NAME << std::endl;
    std::wcout << L"==========================================" << std::endl;
    std::wcout << L"Developer: " << DEVELOPER << std::endl;
    std::wcout << L"Version: " << GetFileVersion() << std::endl;
    std::wcout << L"Product: " << GetProductName() << std::endl;
    std::wcout << L"Copyright: " << GetLegalCopyright() << std::endl;
    std::wcout << L"Description: " << DESCRIPTION << std::endl;
}
