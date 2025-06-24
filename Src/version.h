#pragma once

#include "pch.h"

namespace Version {
    // 기존 상수들 (하드코딩된 값들)
    const std::wstring PROGRAM_NAME = L"MSG File Parser Tool (iMessage Parser)";
    const std::wstring DEVELOPER = L"real2u2l8";
    const std::wstring DESCRIPTION = L"Advanced MSG file analysis tool with MAPI property parsing";
    
    // 리소스에서 동적으로 버전 정보를 읽어오는 함수들
    std::wstring GetFileVersion();
    std::wstring GetProductVersion();
    std::wstring GetProductName();
    std::wstring GetLegalCopyright();
    std::wstring GetInternalName();
    std::wstring GetOriginalFilename();
    
    // 버전 정보 출력 함수들
    void ShowBanner();
    // void ShowVersion();
    // void ShowFullInfo();
} // namespace Version

