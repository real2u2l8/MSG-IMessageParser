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
            return relativePath; // ��ȯ ���н� ���� ��ȯ
        }
        
        return std::wstring(fullPath);
    }

    bool ValidateFilePath(const std::wstring& filePath) {
        // ���� ���� ���� Ȯ��
        DWORD attrs = GetFileAttributesW(filePath.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES) {
            std::wcerr << L"[ERROR] File does not exist: " << filePath << std::endl;
            return false;
        }
        
        // �������� Ȯ�� (���丮�� �ƴ���)
        if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
            std::wcerr << L"[ERROR] Specified path is a directory: " << filePath << std::endl;
            return false;
        }
        
        // .msg Ȯ���� Ȯ��
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
        
        // ���� ���丮�� ����
        wchar_t currentDir[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, currentDir);
        
        std::wstring fullPath = std::wstring(currentDir) + L"\\" + outputDir;
        
        // ���丮 ����
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
            // �ڵ� ��� ���丮 ����
            outputDir = L"";
            return true; // GenerateOutputDirectory���� ó��
        }
        
        // ���丮 ���� �õ�
        if (!CreateDirectoryW(outputDir.c_str(), nullptr)) {
            DWORD error = GetLastError();
            if (error != ERROR_ALREADY_EXISTS) {
                std::wcerr << L"[ERROR] Failed to create output directory: " << outputDir << std::endl;
                return false;
            }
        }
        
        // ���丮���� Ȯ��
        DWORD attrs = GetFileAttributesW(outputDir.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
            std::wcerr << L"[ERROR] Output path is not a valid directory: " << outputDir << std::endl;
            return false;
        }
        
        return true;
    }

    bool CreateDirectoryRecursive(const std::wstring& path) {
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

    std::wstring MakeSafeFileName(const std::wstring& originalName) {
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

} // namespace Utils
