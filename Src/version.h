#pragma once

#include "pch.h"

namespace Version {
    // ���� ����� (�ϵ��ڵ��� ����)
    const std::wstring PROGRAM_NAME = L"MSG File Parser Tool (iMessage Parser)";
    const std::wstring DEVELOPER = L"real2u2l8";
    const std::wstring DESCRIPTION = L"Advanced MSG file analysis tool with MAPI property parsing";
    
    // ���ҽ����� �������� ���� ������ �о���� �Լ���
    std::wstring GetFileVersion();
    std::wstring GetProductVersion();
    std::wstring GetProductName();
    std::wstring GetLegalCopyright();
    std::wstring GetInternalName();
    std::wstring GetOriginalFilename();
    
    // ���� ���� ��� �Լ���
    void ShowBanner();
    // void ShowVersion();
    // void ShowFullInfo();
} // namespace Version

