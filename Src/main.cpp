#include "pch.h"
#include "analysis.h"

// 관리자 권한 확인 함수
bool IsRunningAsAdministrator() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    
    // Administrators 그룹의 SID 생성
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        
        // 현재 프로세스가 Administrators 그룹에 속하는지 확인
        if (!CheckTokenMembership(NULL, adminGroup, &isAdmin)) {
            isAdmin = FALSE;
        }
        
        FreeSid(adminGroup);
    }
    
    return isAdmin == TRUE;
}

// 사용법 출력 함수
void PrintUsage(const std::wstring& programName) {
    std::wcout << L"MSG File Parser Tool" << std::endl;
    std::wcout << L"Usage: " << programName << L" <MSG_FILE_PATH> [OUTPUT_DIR]" << std::endl;
    std::wcout << L"Example: " << programName << L" C:\\test.msg C:\\output" << std::endl;
    std::wcout << L"Example: " << programName << L" test.msg" << std::endl;
    std::wcout << std::endl;
    std::wcout << L"Features:" << std::endl;
    std::wcout << L"  - Recursively traverse all Storage/Stream in MSG file" << std::endl;
    std::wcout << L"  - Convert Stream data to hex format and save" << std::endl;
    std::wcout << L"  - Save Storage as folders, Stream as .hex files" << std::endl;
    std::wcout << std::endl;
    std::wcout << L"Note: This program requires Administrator privileges to access MSG files." << std::endl;
    std::wcout << L"Please run as Administrator if you encounter access denied errors." << std::endl;
}

// 파일 경로 유효성 검사
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

// 출력 디렉토리 생성 및 검증
bool ValidateAndCreateOutputDir(std::wstring& outputDir) {
    if (outputDir.empty()) {
        // 기본 출력 디렉토리 생성
        outputDir = L"MSG_Dump";
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

// 절대 경로로 변환
std::wstring GetAbsolutePath(const std::wstring& relativePath) {
    wchar_t fullPath[MAX_PATH];
    DWORD result = GetFullPathNameW(relativePath.c_str(), MAX_PATH, fullPath, nullptr);
    
    if (result == 0 || result >= MAX_PATH) {
        return relativePath; // 변환 실패시 원본 반환
    }
    
    return std::wstring(fullPath);
}

// 파일명만 추출
std::wstring GetFileName(const std::wstring& fullPath) {
    size_t lastSlash = fullPath.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        return fullPath.substr(lastSlash + 1);
    }
    return fullPath;
}

int main(int argc, char* argv[]) {
    // 콘솔 출력을 위한 로케일 설정
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
    
    std::wcout << L"=== MSG File Parser Tool ===" << std::endl;
    std::wcout << L"Developer: iMessage Parser" << std::endl;
    std::wcout << L"Version: 1.0" << std::endl;
    std::wcout << std::endl;
    
    // 관리자 권한 확인
    if (!IsRunningAsAdministrator()) {
        std::wcerr << L"[ERROR] Administrator privileges are required to access MSG files." << std::endl;
        std::wcerr << L"[ERROR] Please run this program as Administrator." << std::endl;
        std::wcout << std::endl;
        PrintUsage(L"IMessageParser");
        return 1;
    }
    
    // 디버그: argv[0] 값 확인
    std::wcout << L"Debug - argv[0]: " << argv[0] << std::endl;
    
    // 명령행 인자 검사
    if (argc < 2) {
        std::wcerr << L"[ERROR] MSG file path is required." << std::endl;
        std::wcout << std::endl;
        PrintUsage(L"IMessageParser");
        return 1;
    }
    
    // MSG 파일 경로 (char*를 wstring으로 변환)
    std::wstring msgFilePath;
    int len = MultiByteToWideChar(CP_ACP, 0, argv[1], -1, nullptr, 0);
    if (len > 0) {
        wchar_t* wstr = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, argv[1], -1, wstr, len);
        msgFilePath = wstr;
        delete[] wstr;
    }
    
    // 출력 디렉토리 (선택적)
    std::wstring outputDir;
    if (argc >= 3) {
        int len2 = MultiByteToWideChar(CP_ACP, 0, argv[2], -1, nullptr, 0);
        if (len2 > 0) {
            wchar_t* wstr2 = new wchar_t[len2];
            MultiByteToWideChar(CP_ACP, 0, argv[2], -1, wstr2, len2);
            outputDir = wstr2;
            delete[] wstr2;
        }
    }
    
    // 파일 경로 유효성 검사
    if (!ValidateFilePath(msgFilePath)) {
        return 1;
    }
    
    // 절대 경로로 변환
    msgFilePath = GetAbsolutePath(msgFilePath);
    std::wcout << L"[INFO] MSG file path: " << msgFilePath << std::endl;
    
    // 출력 디렉토리 검증 및 생성
    if (!ValidateAndCreateOutputDir(outputDir)) {
        return 1;
    }
    
    outputDir = GetAbsolutePath(outputDir);
    std::wcout << L"[INFO] Output directory: " << outputDir << std::endl;
    std::wcout << std::endl;
    
    try {
        // MSG 파서 생성 및 실행
        MSGParser parser;
        
        std::wcout << L"[INFO] Starting parsing..." << std::endl;
        std::wcout << std::endl;
        
        bool success = parser.ParseMSGFile(msgFilePath, outputDir);
        
        std::wcout << std::endl;
        if (success) {
            std::wcout << L"[SUCCESS] MSG file parsing completed successfully." << std::endl;
            std::wcout << L"[INFO] Results saved to: " << outputDir << std::endl;
            
            // 명시적 리소스 정리
            std::wcout << L"[INFO] Cleaning up resources..." << std::endl;
            
            return 0;
        } else {
            std::wcerr << L"[ERROR] Error occurred during MSG file parsing." << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::wcerr << L"[ERROR] Exception occurred: ";
        std::wcerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::wcerr << L"[ERROR] Unknown exception occurred." << std::endl;
        return 1;
    }
}

