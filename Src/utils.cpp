#include "utils.h"

namespace Utils {

    std::wstring GetCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        
        std::wstringstream ss;
        ss << std::put_time(&tm, L"%Y%m%d_%H%M%S");
        return ss.str();
    }

    std::wstring GetFileNameWithoutExtension(const std::wstring& fullPath) {
        std::wstring fileName = GetFileName(fullPath);
        size_t lastDot = fileName.find_last_of(L".");
        if (lastDot != std::wstring::npos) {
            return fileName.substr(0, lastDot);
        }
        return fileName;
    }

    std::wstring GetFileName(const std::wstring& fullPath) {
        size_t lastSlash = fullPath.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            return fullPath.substr(lastSlash + 1);
        }
        return fullPath;
    }

    std::wstring GetAbsolutePath(const std::wstring& relativePath) {
        wchar_t fullPath[MAX_PATH];
        DWORD result = GetFullPathNameW(relativePath.c_str(), MAX_PATH, fullPath, nullptr);
        
        if (result == 0 || result >= MAX_PATH) {
            return relativePath; // 변환 실패시 원본 반환
        }
        
        return std::wstring(fullPath);
    }

    bool ValidateFilePath(const std::wstring& filePath) {
        // 파일 존재 여부 확인
        DWORD attrs = GetFileAttributesW(filePath.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES) {
            std::wcerr << L"[ERROR] File does not exist: " << filePath << std::endl;
            return false;
        }
        
        // 파일인지 확인 (디렉토리가 아닌지)
        if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
            std::wcerr << L"[ERROR] Specified path is a directory: " << filePath << std::endl;
            return false;
        }
        
        // .msg 확장자 확인
        std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        if (extension != L".msg") {
            std::wcerr << L"[ERROR] Not an MSG file: " << filePath << std::endl;
            std::wcerr << L"[ERROR] Only .msg extension files are supported." << std::endl;
            return false;
        }
        
        return true;
    }

    std::wstring GenerateOutputDirectory(const std::wstring& msgFilePath) {
        std::wstring dateTime = GetCurrentDateTime();
        std::wstring fileName = GetFileNameWithoutExtension(msgFilePath);
        std::wstring outputDir = dateTime + L"_" + fileName + L"_data";
        
        // 현재 디렉토리에 생성
        wchar_t currentDir[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, currentDir);
        
        std::wstring fullPath = std::wstring(currentDir) + L"\\" + outputDir;
        
        // 디렉토리 생성
        if (!CreateDirectoryW(fullPath.c_str(), nullptr)) {
            DWORD error = GetLastError();
            if (error != ERROR_ALREADY_EXISTS) {
                std::wcerr << L"[ERROR] Failed to create output directory: " << fullPath << std::endl;
                return L"";
            }
        }
        
        return fullPath;
    }

    bool ValidateAndCreateOutputDir(std::wstring& outputDir) {
        if (outputDir.empty()) {
            // 자동 출력 디렉토리 생성
            outputDir = L"";
            return true; // GenerateOutputDirectory에서 처리
        }
        
        // 디렉토리 생성 시도
        if (!CreateDirectoryW(outputDir.c_str(), nullptr)) {
            DWORD error = GetLastError();
            if (error != ERROR_ALREADY_EXISTS) {
                std::wcerr << L"[ERROR] Failed to create output directory: " << outputDir << std::endl;
                return false;
            }
        }
        
        // 디렉토리인지 확인
        DWORD attrs = GetFileAttributesW(outputDir.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
            std::wcerr << L"[ERROR] Output path is not a valid directory: " << outputDir << std::endl;
            return false;
        }
        
        return true;
    }

    bool CreateDirectoryRecursive(const std::wstring& path) {
        if (path.empty()) return false;
        
        // 이미 존재하는지 확인
        DWORD attrs = GetFileAttributesW(path.c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
            return true;  // 이미 존재함
        }
        
        // 상위 디렉토리 생성
        size_t lastSlash = path.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            std::wstring parentPath = path.substr(0, lastSlash);
            if (!CreateDirectoryRecursive(parentPath)) {
                return false;
            }
        }
        
        // 현재 디렉토리 생성
        return CreateDirectoryW(path.c_str(), nullptr) == TRUE || GetLastError() == ERROR_ALREADY_EXISTS;
    }

    std::wstring MakeSafeFileName(const std::wstring& originalName) {
        std::wstring safeName = originalName;
        
        // Windows 파일명에서 사용할 수 없는 문자들을 언더스코어로 변경
        const std::wstring invalidChars = L"<>:\"/\\|?*";
        for (wchar_t c : invalidChars) {
            std::replace(safeName.begin(), safeName.end(), c, L'_');
        }
        
        // 파일명이 너무 길면 자르기 (Windows 경로 제한 고려)
        if (safeName.length() > 200) {
            safeName = safeName.substr(0, 200);
        }
        
        return safeName;
    }

} // namespace Utils
