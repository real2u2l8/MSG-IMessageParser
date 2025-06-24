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

#include "mapiparser.h"
#include "filehandle.h"

MAPIPropertyParser::MAPIPropertyParser() : m_debugEnabled(true), m_logLevel(3) {
    LogInfo(L"MAPIPropertyParser initialized");
}

MAPIPropertyParser::~MAPIPropertyParser() {
    for (auto& prop : m_properties) {
        if (prop.Data) {
            delete[] prop.Data;
        }
    }
    LogDebug(L"MAPIPropertyParser destroyed");
}

void MAPIPropertyParser::LogDebug(const std::wstring& message) {
    if (m_debugEnabled && m_logLevel >= 3) {
        std::wcout << L"[DEBUG] " << message << std::endl;
    }
}

void MAPIPropertyParser::LogInfo(const std::wstring& message) {
    if (m_debugEnabled && m_logLevel >= 2) {
        std::wcout << L"[INFO] " << message << std::endl;
    }
}

void MAPIPropertyParser::LogWarning(const std::wstring& message) {
    if (m_debugEnabled && m_logLevel >= 1) {
        std::wcerr << L"[WARN] " << message << std::endl;
    }
}

void MAPIPropertyParser::LogError(const std::wstring& message) {
    if (m_debugEnabled && m_logLevel >= 0) {
        std::wcerr << L"[ERROR] " << message << std::endl;
    }
}

void MAPIPropertyParser::LogHexDump(const std::wstring& prefix, const BYTE* data, DWORD size, DWORD maxBytes) {
    if (!m_debugEnabled || m_logLevel < 3 || !data || size == 0) {
        return;
    }
    
    std::wcout << L"[DEBUG] " << prefix << L" (" << size << L" bytes): ";
    
    DWORD bytesToShow = (size < maxBytes) ? size : maxBytes;
    for (DWORD i = 0; i < bytesToShow; i++) {
        std::wcout << std::hex << std::setw(2) << std::setfill(L'0') << (int)data[i];
        if (i < bytesToShow - 1) std::wcout << L" ";
    }
    
    if (size > maxBytes) {
        std::wcout << L"...";
    }
    std::wcout << std::endl;
}

bool MAPIPropertyParser::ParsePropertiesStream(const std::vector<BYTE>& streamData) {
    LogInfo(L"Starting to parse MAPI Property Stream");
    LogDebug(L"Stream data size: " + std::to_wstring(streamData.size()) + L" bytes");
    
    if (streamData.size() < 16) {
        LogError(L"Stream data too small, minimum 16 bytes required for first property definition");
        return false;
    }
    
    LogDebug(L"First 64 bytes as hex dump:");
    for (int i = 0; i < (64 < (int)streamData.size() ? 64 : (int)streamData.size()); i++) {
        if (i % 16 == 0) LogDebug(L"");
        LogDebug(L" 0x" + std::to_wstring((int)streamData[i]));
    }
    
    LogInfo(L"Parsing 16-byte MAPI Property definitions");
    
    DWORD definitionCount = streamData.size() / 16;
    LogInfo(L"Calculated definition count: " + std::to_wstring(definitionCount));
    
    DWORD firstOffset = streamData[4] | (streamData[5] << 8) | 
                        (streamData[6] << 16) | (streamData[7] << 24);
    LogInfo(L"Value Section starts at offset: " + std::to_wstring(firstOffset));
    
    for (DWORD i = 0; i < definitionCount && (i * 16 + 15) < streamData.size(); i++) {
        DWORD offset = i * 16;
        LogDebug(L"Parsing definition " + std::to_wstring(i + 1) + L" at offset " + std::to_wstring(offset));

        DWORD valueOffset = streamData[offset] | (streamData[offset + 1] << 8) |
                            (streamData[offset + 2] << 16) | (streamData[offset + 3] << 24);
        DWORD flags = streamData[offset + 4] | (streamData[offset + 5] << 8) |
                      (streamData[offset + 6] << 16) | (streamData[offset + 7] << 24);
        DWORD propertyTag = streamData[offset + 8] | (streamData[offset + 9] << 8) |
                            (streamData[offset + 10] << 16) | (streamData[offset + 11] << 24);
        DWORD valueSize = streamData[offset + 12] | (streamData[offset + 13] << 8) |
                          (streamData[offset + 14] << 16) | (streamData[offset + 15] << 24);

        WORD propertyId = (WORD)(propertyTag & 0xFFFF);
        WORD propertyType = (WORD)((propertyTag >> 16) & 0xFFFF);

        LogDebug(L"Definition " + std::to_wstring(i + 1) + L":");
        LogDebug(L"  PropertyTag: 0x" + std::to_wstring(propertyTag) +
                 L" (ID: 0x" + std::to_wstring(propertyId) + L", Type: 0x" + std::to_wstring(propertyType) + L")");
        LogDebug(L"  PropertyName: " + PropertyTagToString(propertyId));
        LogDebug(L"  PropertyType: " + PropertyTypeToString(propertyType));
        LogDebug(L"  Flags: 0x" + std::to_wstring(flags));
        LogDebug(L"  ValueOffset: " + std::to_wstring(valueOffset));
        LogDebug(L"  ValueSize: " + std::to_wstring(valueSize));

        if (valueOffset + valueSize <= streamData.size()) {
            LogDebug(L"Extracting value data from offset " + std::to_wstring(valueOffset) +
                     L" with size " + std::to_wstring(valueSize));

            PROPERTY_VALUE propValue;
            propValue.PropertyTag = propertyId;
            propValue.PropertyType = propertyType;
            propValue.Size = valueSize;
            propValue.Data = new BYTE[valueSize];
            memcpy(propValue.Data, &streamData[valueOffset], valueSize);

            LogHexDump(L"Value data for " + PropertyTagToString(propertyId), propValue.Data, valueSize);

            m_properties.push_back(propValue);
            LogDebug(L"Successfully added property: " + PropertyTagToString(propertyId));

            if (propertyId == PR_RECIPIENT_TYPE && valueSize >= 4) {
                DWORD recipientType = 0;
                memcpy(&recipientType, propValue.Data, 4);
                LogInfo(L"  *** PR_RECIPIENT_TYPE found with value: " + std::to_wstring(recipientType));
                switch (recipientType) {
                    case 1: LogInfo(L"    -> Recipient Type: TO"); break;
                    case 2: LogInfo(L"    -> Recipient Type: CC"); break;
                    case 3: LogInfo(L"    -> Recipient Type: BCC"); break;
                    default: LogInfo(L"    -> Recipient Type: UNKNOWN"); break;
                }
            }
        } else {
            LogError(L"Value data out of bounds - Offset: " + std::to_wstring(valueOffset) +
                     L", Size: " + std::to_wstring(valueSize) +
                     L", Stream size: " + std::to_wstring(streamData.size()));
        }
    }
    
    LogInfo(L"Parsing completed - Found " + std::to_wstring(m_properties.size()) + L" properties");
    return true;
}

bool MAPIPropertyParser::ParseFromHexFile(const std::wstring& hexFilePath) {
    LogInfo(L"Reading hex file: " + hexFilePath);
    
    SafeFileHandle fileHandle(hexFilePath, std::ios::binary);
    if (!fileHandle.is_open()) {
        LogError(L"Failed to open hex file: " + hexFilePath);
        return false;
    }
    
    std::vector<BYTE> streamData;
    std::string line;
    
    while (std::getline(fileHandle.get(), line)) {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        
        for (size_t i = 0; i < line.length(); i += 2) {
            if (i + 1 < line.length()) {
                std::string hexByte = line.substr(i, 2);
                try {
                    BYTE byteValue = (BYTE)std::stoi(hexByte, nullptr, 16);
                    streamData.push_back(byteValue);
                } catch (const std::exception& e) {
                    LogWarning(L"Invalid hex value: " + std::wstring(hexByte.begin(), hexByte.end()));
                }
            }
        }
    }
    
    LogInfo(L"Read " + std::to_wstring(streamData.size()) + L" bytes from hex file");
    
    return ParsePropertiesStream(streamData);
}

std::wstring MAPIPropertyParser::GetStringValue(WORD propertyTag) {
    for (const auto& prop : m_properties) {
        if (prop.PropertyTag == propertyTag) {
            return InterpretValue(prop);
        }
    }
    return L"";
}

DWORD MAPIPropertyParser::GetLongValue(WORD propertyTag) {
    for (const auto& prop : m_properties) {
        if (prop.PropertyTag == propertyTag && prop.Size >= 4) {
            DWORD value = 0;
            memcpy(&value, prop.Data, 4);
            return value;
        }
    }
    return 0;
}

std::vector<BYTE> MAPIPropertyParser::GetBinaryValue(WORD propertyTag) {
    for (const auto& prop : m_properties) {
        if (prop.PropertyTag == propertyTag) {
            return std::vector<BYTE>(prop.Data, prop.Data + prop.Size);
        }
    }
    return std::vector<BYTE>();
}

void MAPIPropertyParser::PrintParsedProperties() {
    std::wcout << L"\n=== Parsed MAPI Properties ===" << std::endl;
    
    for (const auto& prop : m_properties) {
        std::wcout << L"Tag: 0x" << std::hex << prop.PropertyTag 
                   << L" (" << PropertyTagToString(prop.PropertyTag) << L")" << std::endl;
        std::wcout << L"Type: 0x" << std::hex << prop.PropertyType 
                   << L" (" << PropertyTypeToString(prop.PropertyType) << L")" << std::endl;
        std::wcout << L"Size: " << std::dec << prop.Size << L" bytes" << std::endl;
        std::wcout << L"Value: " << InterpretValue(prop) << std::endl;
        std::wcout << L"---" << std::endl;
    }
}

std::wstring MAPIPropertyParser::InterpretValue(const PROPERTY_VALUE& value) {
    switch (value.PropertyType) {
        case PT_STRING8:
            return InterpretString8(value.Data, value.Size);
        case PT_UNICODE:
            return InterpretUnicode(value.Data, value.Size);
        case PT_LONG:
            return InterpretLong(value.Data, value.Size);
        case PT_BINARY:
            return InterpretBinary(value.Data, value.Size);
        case PT_BOOLEAN:
            return value.Size >= 2 ? (*(WORD*)value.Data ? L"True" : L"False") : L"Unknown";
        default:
            return L"[Unsupported Type]";
    }
}

std::wstring MAPIPropertyParser::InterpretString8(const BYTE* data, DWORD size) {
    if (size == 0) return L"";
    
    int len = MultiByteToWideChar(CP_ACP, 0, (LPCCH)data, size, nullptr, 0);
    if (len > 0) {
        wchar_t* wstr = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, (LPCCH)data, size, wstr, len);
        std::wstring result(wstr);
        delete[] wstr;
        return result;
    }
    return L"[String8 Conversion Failed]";
}

std::wstring MAPIPropertyParser::InterpretUnicode(const BYTE* data, DWORD size) {
    if (size == 0) return L"";
    
    size_t charCount = size / 2;
    return std::wstring((wchar_t*)data, charCount);
}

std::wstring MAPIPropertyParser::InterpretBinary(const BYTE* data, DWORD size) {
    if (size == 0) return L"[Empty Binary]";
    
    std::wstringstream ss;
    ss << L"[Binary " << size << L" bytes: ";
    
    DWORD maxShow = (size < 16) ? size : 16;
    for (DWORD i = 0; i < maxShow; i++) {
        ss << std::hex << std::setw(2) << std::setfill(L'0') << (int)data[i];
        if (i < maxShow - 1) ss << L" ";
    }
    
    if (size > 16) ss << L"...";
    ss << L"]";
    
    return ss.str();
}

std::wstring MAPIPropertyParser::InterpretLong(const BYTE* data, DWORD size) {
    if (size >= 4) {
        DWORD value = 0;
        memcpy(&value, data, 4);
        return std::to_wstring(value);
    }
    return L"[Invalid Long]";
}

std::wstring MAPIPropertyParser::PropertyTagToString(WORD propertyTag) {
    switch (propertyTag) {
        case PR_RECIPIENT_TYPE: return L"PR_RECIPIENT_TYPE";
        case PR_DISPLAY_NAME: return L"PR_DISPLAY_NAME";
        case PR_SMTP_ADDRESS: return L"PR_SMTP_ADDRESS";
        case PR_EMAIL_ADDRESS: return L"PR_EMAIL_ADDRESS";
        case PR_ADDRTYPE: return L"PR_ADDRTYPE";
        case PR_ENTRYID: return L"PR_ENTRYID";
        case PR_SEARCH_KEY: return L"PR_SEARCH_KEY";
        default: return L"UNKNOWN_TAG";
    }
}

std::wstring MAPIPropertyParser::PropertyTypeToString(WORD propertyType) {
    switch (propertyType) {
        case PT_UNSPECIFIED: return L"PT_UNSPECIFIED";
        case PT_NULL: return L"PT_NULL";
        case PT_I2: return L"PT_I2";
        case PT_LONG: return L"PT_LONG";
        case PT_R4: return L"PT_R4";
        case PT_DOUBLE: return L"PT_DOUBLE";
        case PT_CURRENCY: return L"PT_CURRENCY";
        case PT_APPTIME: return L"PT_APPTIME";
        case PT_ERROR: return L"PT_ERROR";
        case PT_BOOLEAN: return L"PT_BOOLEAN";
        case PT_OBJECT: return L"PT_OBJECT";
        case PT_I8: return L"PT_I8";
        case PT_STRING8: return L"PT_STRING8";
        case PT_UNICODE: return L"PT_UNICODE";
        case PT_SYSTIME: return L"PT_SYSTIME";
        case PT_CLSID: return L"PT_CLSID";
        case PT_BINARY: return L"PT_BINARY";
        default: return L"UNKNOWN_TYPE";
    }
} 