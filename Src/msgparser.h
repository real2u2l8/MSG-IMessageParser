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

class MSGParser {
private:
    std::wstring m_outputBaseDir;
    int m_fileCount;
    int m_errorCount;

    void TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir);
    bool SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath);
    std::string BinaryToHexString(const BYTE* data, DWORD size);
    std::wstring MakeSafeFileName(const std::wstring& originalName);
    bool CreateDirectoryRecursive(const std::wstring& path);
    void LogMessage(const std::wstring& message);
    void LogError(const std::wstring& error);

public:
    MSGParser();
    ~MSGParser();
    bool ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir);
}; 