/*
 * MSG File Parser Tool - Advanced MSG file analysis tool with MAPI property parsing
 * Copyright (C) 2025  real2u2l8
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "pch.h"

namespace Version {
    // 상수 함수들 (하드코딩된 값들)
    const std::wstring PROGRAM_NAME = L"MSG File Parser Tool (iMessage Parser)";
    const std::wstring DEVELOPER = L"real2u2l8";
    const std::wstring DESCRIPTION = L"Advanced MSG file analysis tool with MAPI property parsing";
    
    // 파일에서 동적으로 버전 정보를 읽어오는 함수들
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

