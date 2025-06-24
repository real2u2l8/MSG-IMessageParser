#include "analysis.h"

// MSGParser Ŭ���� ����
MSGParser::MSGParser() : m_fileCount(0), m_errorCount(0) {
    // COM �ʱ�ȭ
    CoInitialize(nullptr);
}

MSGParser::~MSGParser() {
    // ��� ���ҽ� ����
    LogMessage(L"MSGParser destructor called - cleaning up resources");
    
    // COM ����
    CoUninitialize();
    
    LogMessage(L"MSGParser cleanup completed");
}

bool MSGParser::ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir) {
    LogMessage(L"MSG file parsing started: " + filePath);
    
    m_outputBaseDir = outputDir;
    m_fileCount = 0;
    m_errorCount = 0;
    
    // ��� ���丮 ����
    if (!CreateDirectoryRecursive(outputDir)) {
        LogError(L"Failed to create output directory: " + outputDir);
        return false;
    }
    
    // IStorage�� MSG ���� ����
    IStorage* pStorage = nullptr;
    HRESULT hr = StgOpenStorage(
        filePath.c_str(),
        nullptr,
        STGM_READ | STGM_SHARE_DENY_NONE,  // DENY_WRITE ��� DENY_NONE ���
        nullptr,
        0,
        &pStorage
    );
    
    if (FAILED(hr) || !pStorage) {
        // �ٸ� ���� ���� ��õ�
        hr = StgOpenStorage(
            filePath.c_str(),
            nullptr,
            STGM_READ | STGM_SHARE_EXCLUSIVE,  // EXCLUSIVE ��� �õ�
            nullptr,
            0,
            &pStorage
        );
        
        if (FAILED(hr) || !pStorage) {
            LogError(L"Failed to open MSG file: " + filePath + L" (HRESULT: 0x" + 
                     std::to_wstring(hr) + L") - Access denied even with different modes");
            return false;
        }
    }
    
    // ��Ʈ ���丮������ ����� Ž�� ����
    TraverseStorage(pStorage, L"", outputDir);
    
    // ���丮�� ����
    pStorage->Release();
    
    LogMessage(L"Parsing completed - Processed files: " + std::to_wstring(m_fileCount) + 
               L", Errors: " + std::to_wstring(m_errorCount));
    
    return m_errorCount == 0;
}

void MSGParser::TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir) {
    if (!pStorage) return;
    
    // ���� ���丮���� ��� ��� ����
    IEnumSTATSTG* pEnum = nullptr;
    HRESULT hr = pStorage->EnumElements(0, nullptr, 0, &pEnum);
    
    if (FAILED(hr) || !pEnum) {
        LogError(L"Failed to enumerate storage: " + currentPath + L" (HRESULT: 0x" + 
                 std::to_wstring(hr) + L")");
        return;
    }
    
    STATSTG stat;
    ULONG fetched;
    
    while (SUCCEEDED(pEnum->Next(1, &stat, &fetched)) && fetched > 0) {
        std::wstring elementName = stat.pwcsName;
        std::wstring fullPath = currentPath.empty() ? elementName : currentPath + L"\\" + elementName;
        std::wstring safeName = MakeSafeFileName(elementName);
        std::wstring elementOutputPath = outputDir + L"\\" + safeName;
        
        LogMessage(L"Processing element: " + elementName + L" (Type: " + 
                   (stat.type == STGTY_STORAGE ? L"Storage" : L"Stream") + L")");
        
        if (stat.type == STGTY_STORAGE) {
            // ���丮���� ��� - ���� ���� �� ��� Ž��
            LogMessage(L"Found storage: " + fullPath);
            
            if (CreateDirectoryRecursive(elementOutputPath)) {
                IStorage* pSubStorage = nullptr;
                // �ٸ� ���� ��� �õ�
                hr = pStorage->OpenStorage(
                    stat.pwcsName,
                    nullptr,
                    STGM_READ | STGM_SHARE_DENY_NONE,  // DENY_WRITE ��� DENY_NONE ���
                    nullptr,
                    0,
                    &pSubStorage
                );
                
                if (SUCCEEDED(hr) && pSubStorage) {
                    TraverseStorage(pSubStorage, fullPath, elementOutputPath);
                    pSubStorage->Release();
                } else {
                    // �ٸ� ���� ���� ��õ�
                    hr = pStorage->OpenStorage(
                        stat.pwcsName,
                        nullptr,
                        STGM_READ | STGM_SHARE_EXCLUSIVE,  // EXCLUSIVE ��� �õ�
                        nullptr,
                        0,
                        &pSubStorage
                    );
                    
                    if (SUCCEEDED(hr) && pSubStorage) {
                        TraverseStorage(pSubStorage, fullPath, elementOutputPath);
                        pSubStorage->Release();
                    } else {
                        LogError(L"Failed to open sub storage: " + fullPath + L" (HRESULT: 0x" + 
                                 std::to_wstring(hr) + L") - Access denied even with different modes");
                        m_errorCount++;
                    }
                }
            } else {
                LogError(L"Failed to create directory: " + elementOutputPath);
                m_errorCount++;
            }
        }
        else if (stat.type == STGTY_STREAM) {
            // ��Ʈ���� ��� - hex ���Ϸ� ����
            LogMessage(L"Found stream: " + fullPath);
            
            IStream* pStream = nullptr;
            // �ٸ� ���� ��� �õ�
            hr = pStorage->OpenStream(
                stat.pwcsName,
                nullptr,
                STGM_READ | STGM_SHARE_DENY_NONE,  // DENY_WRITE ��� DENY_NONE ���
                0,
                &pStream
            );
            
            if (SUCCEEDED(hr) && pStream) {
                std::wstring hexFilePath = elementOutputPath + L".hex";
                if (SaveStreamAsHex(pStream, hexFilePath)) {
                    m_fileCount++;
                } else {
                    m_errorCount++;
                }
                pStream->Release();
            } else {
                // �ٸ� ���� ���� ��õ�
                hr = pStorage->OpenStream(
                    stat.pwcsName,
                    nullptr,
                    STGM_READ | STGM_SHARE_EXCLUSIVE,  // EXCLUSIVE ��� �õ�
                    0,
                    &pStream
                );
                
                if (SUCCEEDED(hr) && pStream) {
                    std::wstring hexFilePath = elementOutputPath + L".hex";
                    if (SaveStreamAsHex(pStream, hexFilePath)) {
                        m_fileCount++;
                    } else {
                        m_errorCount++;
                    }
                    pStream->Release();
                } else {
                    LogError(L"Failed to open stream: " + fullPath + L" (HRESULT: 0x" + 
                             std::to_wstring(hr) + L") - Access denied even with different modes");
                    m_errorCount++;
                }
            }
        }
        
        // �޸� ����
        if (stat.pwcsName) {
            CoTaskMemFree(stat.pwcsName);
        }
    }
    
    pEnum->Release();
}

bool MSGParser::SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath) {
    if (!pStream) return false;
    
    // ��Ʈ�� ũ�� Ȯ��
    STATSTG stat;
    HRESULT hr = pStream->Stat(&stat, STATFLAG_NONAME);
    if (FAILED(hr)) {
        LogError(L"Failed to get stream size: " + outputPath);
        return false;
    }
    
    DWORD streamSize = stat.cbSize.LowPart;
    if (streamSize == 0) {
        LogMessage(L"Skipping empty stream: " + outputPath);
        return true;
    }
    
    // ��Ʈ�� ������ �б�
    std::vector<BYTE> buffer(streamSize);
    ULONG bytesRead = 0;
    
    hr = pStream->Read(buffer.data(), streamSize, &bytesRead);
    if (FAILED(hr) || bytesRead != streamSize) {
        LogError(L"Failed to read stream data: " + outputPath);
        return false;
    }
    
    // ���Ϸ� ���� ���� (.dat Ȯ����)
    std::wstring datPath = outputPath.substr(0, outputPath.find_last_of(L'.')) + L".dat";
    std::ofstream file(datPath, std::ios::binary);
    if (!file.is_open()) {
        LogError(L"Failed to create dat file: " + datPath);
        return false;
    }
    file.write(reinterpret_cast<const char*>(buffer.data()), bytesRead);
    file.close();
    
    LogMessage(L"Binary file saved: " + datPath + L" (" + std::to_wstring(bytesRead) + L" bytes)");
    return true;
}

std::string MSGParser::BinaryToHexString(const BYTE* data, DWORD size) {
    if (!data || size == 0) return "";
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::uppercase;
    
    for (DWORD i = 0; i < size; i++) {
        if (i > 0 && i % 16 == 0) {
            ss << std::endl;  // 16����Ʈ���� �ٹٲ�
        }
        ss << std::setw(2) << static_cast<int>(data[i]);
        
        if (i < size - 1 && (i + 1) % 16 != 0) {
            ss << " ";  // ����Ʈ ���� ����
        }
    }
    
    return ss.str();
}

std::wstring MSGParser::MakeSafeFileName(const std::wstring& originalName) {
    std::wstring safeName = originalName;
    
    // Windows ���ϸ��� ����� �� ���� ���ڵ��� ������ھ�� ����
    const std::wstring invalidChars = L"<>:\"/\\|?*";
    for (wchar_t c : invalidChars) {
        std::replace(safeName.begin(), safeName.end(), c, L'_');
    }
    
    // ���ϸ��� �ʹ� ��� �ڸ��� (Windows ��� ���� ���)
    if (safeName.length() > 200) {
        safeName = safeName.substr(0, 200);
    }
    
    return safeName;
}

bool MSGParser::CreateDirectoryRecursive(const std::wstring& path) {
    if (path.empty()) return false;
    
    // �̹� �����ϴ��� Ȯ��
    DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        return true;  // �̹� ������
    }
    
    // ���� ���丮 ����
    size_t lastSlash = path.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        std::wstring parentPath = path.substr(0, lastSlash);
        if (!CreateDirectoryRecursive(parentPath)) {
            return false;
        }
    }
    
    // ���� ���丮 ����
    return CreateDirectoryW(path.c_str(), nullptr) == TRUE || GetLastError() == ERROR_ALREADY_EXISTS;
}

void MSGParser::LogMessage(const std::wstring& message) {
    std::wcout << L"[INFO] " << message << std::endl;
}

void MSGParser::LogError(const std::wstring& error) {
    std::wcerr << L"[ERROR] " << error << std::endl;
}

// MAPI Property Parser ����
MAPIPropertyParser::MAPIPropertyParser() : m_debugEnabled(true), m_logLevel(3) {
    LogInfo(L"MAPIPropertyParser initialized");
}

MAPIPropertyParser::~MAPIPropertyParser() {
    // ���� �Ҵ�� ������ ����
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
    
    // ��ü ������ ���� �м� (����׿�)
    LogDebug(L"First 64 bytes as hex dump:");
    for (int i = 0; i < (64 < (int)streamData.size() ? 64 : (int)streamData.size()); i++) {
        if (i % 16 == 0) LogDebug(L"");
        LogDebug(L" 0x" + std::to_wstring((int)streamData[i]));
    }
    
    // 16����Ʈ MAPI Property Stream ����:
    // 4 bytes: PropertyTag (��: 0x0C150003 = PR_RECIPIENT_TYPE + PT_LONG)
    // 4 bytes: Flags (0x00000006 ��, Reserved/Presence Flag)
    // 4 bytes: Value Offset (Value Section �� ��ġ)
    // 4 bytes: Value Size (���� ����)
    
    LogInfo(L"Parsing 16-byte MAPI Property definitions");
    
    DWORD definitionCount = streamData.size() / 16;
    LogInfo(L"Calculated definition count: " + std::to_wstring(definitionCount));
    
    // Value Section ���� ��ġ ã�� (ù ��° ������ Offset ��)
    DWORD firstOffset = streamData[4] | (streamData[5] << 8) | 
                        (streamData[6] << 16) | (streamData[7] << 24);
    LogInfo(L"Value Section starts at offset: " + std::to_wstring(firstOffset));
    
    // Property Definition Section �Ľ�
    for (DWORD i = 0; i < definitionCount && (i * 16 + 15) < streamData.size(); i++) {
        DWORD offset = i * 16;
        LogDebug(L"Parsing definition " + std::to_wstring(i + 1) + L" at offset " + std::to_wstring(offset));

        // Property Definition ������ �°� �Ľ�
        DWORD valueOffset = streamData[offset] | (streamData[offset + 1] << 8) |
                            (streamData[offset + 2] << 16) | (streamData[offset + 3] << 24);
        DWORD flags = streamData[offset + 4] | (streamData[offset + 5] << 8) |
                      (streamData[offset + 6] << 16) | (streamData[offset + 7] << 24);
        DWORD propertyTag = streamData[offset + 8] | (streamData[offset + 9] << 8) |
                            (streamData[offset + 10] << 16) | (streamData[offset + 11] << 24);
        DWORD valueSize = streamData[offset + 12] | (streamData[offset + 13] << 8) |
                          (streamData[offset + 14] << 16) | (streamData[offset + 15] << 24);

        // PropertyTag���� Property ID�� Type �и�
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

        // Value Section���� ���� ������ ����
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

            // Ư���� �ؼ� (PR_RECIPIENT_TYPE)
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
    
    // RAII ���۸� ����Ͽ� ������ ���� �ڵ鸵
    SafeFileHandle fileHandle(hexFilePath, std::ios::binary);
    if (!fileHandle.is_open()) {
        LogError(L"Failed to open hex file: " + hexFilePath);
        return false;
    }
    
    std::vector<BYTE> streamData;
    std::string line;
    
    // hex ������ �� ���� �о ����Ʈ�� ��ȯ
    while (std::getline(fileHandle.get(), line)) {
        // ���� ����
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        
        // 2�ڸ��� hex ���� �о ����Ʈ�� ��ȯ
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
    
    // RAII ���۰� �ڵ����� ������ ����
    LogInfo(L"Read " + std::to_wstring(streamData.size()) + L" bytes from hex file");
    
    // ���� �����ͷ� �Ľ� ����
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
    
    // 8-bit ���ڿ��� �����ڵ�� ��ȯ
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
    
    // Unicode ���ڿ� (2����Ʈ��)
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

// SafeFileHandle ���� �߰�
SafeFileHandle::SafeFileHandle(const std::wstring& path, std::ios::openmode mode)
    : m_path(path), m_mode(mode) {
    m_file.open(path, mode);
}

SafeFileHandle::~SafeFileHandle() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

bool SafeFileHandle::is_open() const {
    return m_file.is_open();
}

std::ifstream& SafeFileHandle::get() {
    return m_file;
}
