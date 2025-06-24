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

#include "msgparser.h"

MSGParser::MSGParser() : m_fileCount(0), m_errorCount(0) {
    CoInitialize(nullptr);
}

MSGParser::~MSGParser() {
    LogMessage(L"MSGParser destructor called - cleaning up resources");
    CoUninitialize();
    LogMessage(L"MSGParser cleanup completed");
}

bool MSGParser::ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir) {
    LogMessage(L"MSG file parsing started: " + filePath);
    
    m_outputBaseDir = outputDir;
    m_fileCount = 0;
    m_errorCount = 0;
    
    if (!CreateDirectoryRecursive(outputDir)) {
        LogError(L"Failed to create output directory: " + outputDir);
        return false;
    }
    
    IStorage* pStorage = nullptr;
    HRESULT hr = StgOpenStorage(
        filePath.c_str(),
        nullptr,
        STGM_READ | STGM_SHARE_DENY_NONE,
        nullptr,
        0,
        &pStorage
    );
    
    if (FAILED(hr) || !pStorage) {
        hr = StgOpenStorage(
            filePath.c_str(),
            nullptr,
            STGM_READ | STGM_SHARE_EXCLUSIVE,
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
    
    TraverseStorage(pStorage, L"", outputDir);
    pStorage->Release();
    
    LogMessage(L"Parsing completed - Processed files: " + std::to_wstring(m_fileCount) + 
               L", Errors: " + std::to_wstring(m_errorCount));
    
    return m_errorCount == 0;
}

void MSGParser::TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir) {
    if (!pStorage) return;
    
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
            LogMessage(L"Found storage: " + fullPath);
            
            if (CreateDirectoryRecursive(elementOutputPath)) {
                IStorage* pSubStorage = nullptr;
                hr = pStorage->OpenStorage(
                    stat.pwcsName,
                    nullptr,
                    STGM_READ | STGM_SHARE_DENY_NONE,
                    nullptr,
                    0,
                    &pSubStorage
                );
                
                if (SUCCEEDED(hr) && pSubStorage) {
                    TraverseStorage(pSubStorage, fullPath, elementOutputPath);
                    pSubStorage->Release();
                } else {
                    hr = pStorage->OpenStorage(
                        stat.pwcsName,
                        nullptr,
                        STGM_READ | STGM_SHARE_EXCLUSIVE,
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
            LogMessage(L"Found stream: " + fullPath);
            
            IStream* pStream = nullptr;
            hr = pStorage->OpenStream(
                stat.pwcsName,
                nullptr,
                STGM_READ | STGM_SHARE_DENY_NONE,
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
                hr = pStorage->OpenStream(
                    stat.pwcsName,
                    nullptr,
                    STGM_READ | STGM_SHARE_EXCLUSIVE,
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
        
        if (stat.pwcsName) {
            CoTaskMemFree(stat.pwcsName);
        }
    }
    
    pEnum->Release();
}

bool MSGParser::SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath) {
    if (!pStream) return false;
    
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
    
    std::vector<BYTE> buffer(streamSize);
    ULONG bytesRead = 0;
    
    hr = pStream->Read(buffer.data(), streamSize, &bytesRead);
    if (FAILED(hr) || bytesRead != streamSize) {
        LogError(L"Failed to read stream data: " + outputPath);
        return false;
    }
    
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
            ss << std::endl;
        }
        ss << std::setw(2) << static_cast<int>(data[i]);
        
        if (i < size - 1 && (i + 1) % 16 != 0) {
            ss << " ";
        }
    }
    
    return ss.str();
}

std::wstring MSGParser::MakeSafeFileName(const std::wstring& originalName) {
    std::wstring safeName = originalName;
    
    const std::wstring invalidChars = L"<>:\"/\\|?*";
    for (wchar_t c : invalidChars) {
        std::replace(safeName.begin(), safeName.end(), c, L'_');
    }
    
    if (safeName.length() > 200) {
        safeName = safeName.substr(0, 200);
    }
    
    return safeName;
}

bool MSGParser::CreateDirectoryRecursive(const std::wstring& path) {
    if (path.empty()) return false;
    
    DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        return true;
    }
    
    size_t lastSlash = path.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        std::wstring parentPath = path.substr(0, lastSlash);
        if (!CreateDirectoryRecursive(parentPath)) {
            return false;
        }
    }
    
    return CreateDirectoryW(path.c_str(), nullptr) == TRUE || GetLastError() == ERROR_ALREADY_EXISTS;
}

void MSGParser::LogMessage(const std::wstring& message) {
    std::wcout << L"[INFO] " << message << std::endl;
}

void MSGParser::LogError(const std::wstring& error) {
    std::wcerr << L"[ERROR] " << error << std::endl;
} 