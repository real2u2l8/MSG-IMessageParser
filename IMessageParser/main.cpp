#include "pch.h"
#include "analysis.h"

// 사용법 출력 함수
void PrintUsage(const std::wstring& programName) {
    std::wcout << L"MSG 파일 파싱 도구" << std::endl;
    std::wcout << L"사용법: " << programName << L" <MSG파일경로> [출력디렉토리]" << std::endl;
    std::wcout << L"예시: " << programName << L" C:\\test.msg C:\\output" << std::endl;
    std::wcout << L"예시: " << programName << L" test.msg" << std::endl;
    std::wcout << std::endl;
    std::wcout << L"기능:" << std::endl;
    std::wcout << L"  - MSG 파일의 모든 Storage/Stream을 재귀적으로 탐색" << std::endl;
    std::wcout << L"  - Stream 데이터를 hex 형식으로 변환하여 저장" << std::endl;
    std::wcout << L"  - Storage는 폴더로, Stream은 .hex 파일로 저장" << std::endl;
}

// 파일 경로 유효성 검사
bool ValidateFilePath(const std::wstring& filePath) {
    // 파일 존재 여부 확인
    DWORD attrs = GetFileAttributes(filePath.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        std::wcerr << L"[ERROR] 파일이 존재하지 않습니다: " << filePath << std::endl;
        return false;
    }
    
    // 파일인지 확인 (디렉토리가 아닌지)
    if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
        std::wcerr << L"[ERROR] 지정된 경로는 디렉토리입니다: " << filePath << std::endl;
        return false;
    }
    
    // .msg 확장자 확인
    std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    if (extension != L".msg") {
        std::wcerr << L"[ERROR] MSG 파일이 아닙니다: " << filePath << std::endl;
        std::wcerr << L"[ERROR] .msg 확장자 파일만 지원합니다." << std::endl;
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
    if (!CreateDirectory(outputDir.c_str(), nullptr)) {
        DWORD error = GetLastError();
        if (error != ERROR_ALREADY_EXISTS) {
            std::wcerr << L"[ERROR] 출력 디렉토리 생성 실패: " << outputDir << std::endl;
            return false;
        }
    }
    
    // 디렉토리인지 확인
    DWORD attrs = GetFileAttributes(outputDir.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        std::wcerr << L"[ERROR] 출력 경로가 유효한 디렉토리가 아닙니다: " << outputDir << std::endl;
        return false;
    }
    
    return true;
}

// 절대 경로로 변환
std::wstring GetAbsolutePath(const std::wstring& relativePath) {
    wchar_t fullPath[MAX_PATH];
    DWORD result = GetFullPathName(relativePath.c_str(), MAX_PATH, fullPath, nullptr);
    
    if (result == 0 || result >= MAX_PATH) {
        return relativePath; // 변환 실패시 원본 반환
    }
    
    return std::wstring(fullPath);
}

int main(int argc, wchar_t* argv[]) {
    // 콘솔 출력을 위한 로케일 설정
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
    
    std::wcout << L"=== MSG 파일 파싱 도구 ===" << std::endl;
    std::wcout << L"개발: iMessage Parser" << std::endl;
    std::wcout << L"버전: 1.0" << std::endl;
    std::wcout << std::endl;
    
    // 명령행 인자 검사
    if (argc < 2) {
        std::wcerr << L"[ERROR] MSG 파일 경로가 필요합니다." << std::endl;
        std::wcout << std::endl;
        PrintUsage(argv[0]);
        return 1;
    }
    
    // MSG 파일 경로
    std::wstring msgFilePath = argv[1];
    
    // 출력 디렉토리 (선택적)
    std::wstring outputDir;
    if (argc >= 3) {
        outputDir = argv[2];
    }
    
    // 파일 경로 유효성 검사
    if (!ValidateFilePath(msgFilePath)) {
        return 1;
    }
    
    // 절대 경로로 변환
    msgFilePath = GetAbsolutePath(msgFilePath);
    std::wcout << L"[INFO] MSG 파일 경로: " << msgFilePath << std::endl;
    
    // 출력 디렉토리 검증 및 생성
    if (!ValidateAndCreateOutputDir(outputDir)) {
        return 1;
    }
    
    outputDir = GetAbsolutePath(outputDir);
    std::wcout << L"[INFO] 출력 디렉토리: " << outputDir << std::endl;
    std::wcout << std::endl;
    
    try {
        // MSG 파서 생성 및 실행
        MSGParser parser;
        
        std::wcout << L"[INFO] 파싱을 시작합니다..." << std::endl;
        std::wcout << std::endl;
        
        bool success = parser.ParseMSGFile(msgFilePath, outputDir);
        
        std::wcout << std::endl;
        if (success) {
            std::wcout << L"[SUCCESS] MSG 파일 파싱이 성공적으로 완료되었습니다." << std::endl;
            std::wcout << L"[INFO] 결과는 다음 디렉토리에 저장되었습니다: " << outputDir << std::endl;
            return 0;
        } else {
            std::wcerr << L"[ERROR] MSG 파일 파싱 중 오류가 발생했습니다." << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::wcerr << L"[ERROR] 예외 발생: ";
        std::wcerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::wcerr << L"[ERROR] 알 수 없는 예외가 발생했습니다." << std::endl;
        return 1;
    }
}

