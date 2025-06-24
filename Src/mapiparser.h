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
#include "analysis.h"

struct PROPERTY_VALUE {
    WORD PropertyTag;
    WORD PropertyType;
    DWORD Size;
    BYTE* Data;
};

class MAPIPropertyParser {
private:
    std::vector<PROPERTY_VALUE> m_properties;
    bool m_debugEnabled;
    int m_logLevel;

    void LogDebug(const std::wstring& message);
    void LogInfo(const std::wstring& message);
    void LogWarning(const std::wstring& message);
    void LogError(const std::wstring& message);
    void LogHexDump(const std::wstring& prefix, const BYTE* data, DWORD size, DWORD maxBytes = 16);
    
    std::wstring InterpretValue(const PROPERTY_VALUE& value);
    std::wstring InterpretString8(const BYTE* data, DWORD size);
    std::wstring InterpretUnicode(const BYTE* data, DWORD size);
    std::wstring InterpretBinary(const BYTE* data, DWORD size);
    std::wstring InterpretLong(const BYTE* data, DWORD size);
    
    std::wstring PropertyTagToString(WORD propertyTag);
    std::wstring PropertyTypeToString(WORD propertyType);

public:
    MAPIPropertyParser();
    ~MAPIPropertyParser();
    
    bool ParsePropertiesStream(const std::vector<BYTE>& streamData);
    bool ParseFromHexFile(const std::wstring& hexFilePath);
    
    std::wstring GetStringValue(WORD propertyTag);
    DWORD GetLongValue(WORD propertyTag);
    std::vector<BYTE> GetBinaryValue(WORD propertyTag);
    
    void PrintParsedProperties();
}; 