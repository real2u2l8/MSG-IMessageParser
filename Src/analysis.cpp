#include "pch.h"
#include "analysis.h"

// OutLook Mail의 .msg 파일의 구조 문서, 여기서 각각의 Substg를 확인할 수 있다. by. real.hansy
// https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
// http://www.five-ten-sg.com/libpst/rn01re06.html
// OutLook Mail의 .msg 파일 구조는 버전에 따라 바뀔 수 있다.
// KESM_OutMail.exe를 통해 만들어지는 .msg 덤프파일은 간혹 이슈가 발생한다.
// 메인 스토리지 내의 필드 
#define		SUBSTG_SENDER		L"__substg1.0_0C1F"		// PR_SENDER_EMAIL_ADDRESS_W:	송신자의 이메일 주소 (Exchange 값일 수도 있음)
#define		SUBSTG_TO			L"__substg1.0_0E04"		// PR_DISPLAY_TO_W:				수신자(To) 전체 표시 문자열,	주소가 아닐 수 있음
#define		SUBSTG_CC			L"__substg1.0_0E03"		// PR_DISPLAY_CC_W:				참조(CC) 전체 표시 문자열		주소가 아닐 수 있음
#define		SUBSTG_BCC			L"__substg1.0_0E02"		// PR_DISPLAY_BCC_W:			숨은참조(BCC) 전체 표시 문자열	주소가 아닐 수 있음
#define		SUBSTG_SUBJECT		L"__substg1.0_0037"		// PR_SUBJECT_W:				메일 제목
#define		SUBSTG_BODY			L"__substg1.0_1000"		// PR_BODY_W:					메일 본문 (텍스트)
#define		SUBSTG_HEADER		L"__substg1.0_007D"		// PR_SUBJECT_PREFIX:			제목 앞 접두어 ("RE:", "FW:" 등)
// 서브 스토리지
#define     MAIL_MAIN			L"__nameid_version"		// Named Property의 이름-GUID-Tag 매핑 정보를 담는 메타 스토리지
#define     MAIL_SUBTO			L"__recip_version"		// 각 수신자(TO/CC/BCC)의 Recipient Table 스토리지
#define		MAIL_PROPERTY		L"__properties_version"	// 서브 스토리지 내의 세부 속성을 담는 스토리지
#define     MAIL_ATTACH			L"__attach_version"		// 첨부파일 관련 속성과 데이터를 담는 Attach 스토리지
// __attach_version 서브 트리
#define		SUBSTG_ATTACHDISPLAY	L"__substg1.0_3001"		// PR_DISPLAY_NAME_W:        첨부파일 표시 이름
#define		SUBSTG_ATTACHNAME		L"__substg1.0_3707"		// PR_ATTACH_LONG_FILENAME_W: 첨부파일 실제 이름 (긴 형식)
#define		SUBSTG_ATTACHDATA		L"__substg1.0_3701"		// PR_ATTACH_DATA_BIN:        첨부파일 바이너리 데이터
// 특수 매크로
#define		SUBSTG_ADDRTYPE		    L"__substg1.0_3002"		// PR_ADDRTYPE_W:             주소 타입 (SMTP, EX 등)
#define		SUBSTG_SENDER_ADDRTYPE	L"__substg1.0_0C1E"		// PR_SENDER_ADDRTYPE_W:      송신자 주소 타입 (SMTP, EX 등)
#define		SUBSTG_SMTPADDRESS		L"__substg1.0_39FE"		// PR_SMTP_ADDRESS_W:         EX 타입의 실제 이메일 주소
#define		SUBSTG_SUBTO		    L"__substg1.0_3003"		// PR_EMAIL_ADDRESS_W:        수신자 이메일 주소
#define		SUBSTG_RECIPIENT_TYPE	L"__substg1.0_0C15"		// PR_RECIPIENT_TYPE:		수신자 역할 구분 (To, Cc, Bcc)
// _recip_version 서브 트리
#define		RECIP_TYPE_TO			1       // 수신자(To)
#define		RECIP_TYPE_CC			2       // 참조(Cc)
#define		RECIP_TYPE_BCC			3       // 숨은참조(Bcc)


#define		MAX_RECIPIENT_ADDR_LEN	256  // TO/CC/BCC 통합용

// 250617.Outmail.exe 메일덤프 분석 이슈. by. real.hansy
// 이메일 주소 타입을 나타내는 열거형
// SMTP: 일반적인 인터넷 이메일 주소 형식 (예: user@domain.com)
// EX: Exchange 서버에서 사용하는 내부 주소 형식
// UNKNOWN: 알 수 없는 주소 형식
enum class AddrType
{
	SMTP,
	EX,
	UNKNOWN
};

// 250618.Outmail.exe 메일덤프 분석 이슈. by. real.hansy
/**
 * @brief 수신자 주소 캐싱 구조체
 *
 * @details
 * - 메일의 수신자(TO, CC, BCC) 정보를 임시로 저장하기 위한 구조체입니다.
 * - 파싱 전에 추출된 원본 문자열을 타입별로 구분하여 보관하며,
 * - 후처리 단계에서 mail_info 구조체로 전달되기 전 ToContentsParse() 함수로 가공됩니다.
 *
 * @note
 * - 수신자 구분을 명확히 하기 위해 TO/CC/BCC를 각각 멤버로 분리하였으며,
 * - 타입값도 함께 보관함으로써 향후 조건 분기 처리나 로깅 등 추가 활용이 용이합니다.
 * - STL map 등의 범용 컨테이너 대신 구조체 고정형을 사용하여 처리 속도와 코드 명료성을 확보하였습니다.
 */
typedef struct _RecipCache {
	char* pszTo;		// To 주소 문자열 (콤마/세미콜론 포함된 원본 그대로)
	char* pszCc;		// Cc 주소 문자열 (콤마/세미콜론 포함된 원본 그대로)
	char* pszBcc;	    // Bcc 주소 문자열 (콤마/세미콜론 포함된 원본 그대로)

	int   nToType;   // 1 = TO
	int   nCcType;   // 2 = CC
	int   nBccType;  // 3 = BCC
} RecipCache;

// 여기에 실제 함수 구현들이 들어갈 예정입니다.
// 현재는 모든 정의가 analysis.h로 이동되었습니다.

// MSGParser 클래스 구현
MSGParser::MSGParser() : m_fileCount(0), m_errorCount(0) {
    // COM 초기화
    CoInitialize(nullptr);
}

MSGParser::~MSGParser() {
    // COM 해제
    CoUninitialize();
}

bool MSGParser::ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir) {
    LogMessage(L"MSG 파일 파싱 시작: " + filePath);
    
    m_outputBaseDir = outputDir;
    m_fileCount = 0;
    m_errorCount = 0;
    
    // 출력 디렉토리 생성
    if (!CreateDirectoryRecursive(outputDir)) {
        LogError(L"출력 디렉토리 생성 실패: " + outputDir);
        return false;
    }
    
    // IStorage로 MSG 파일 열기
    IStorage* pStorage = nullptr;
    HRESULT hr = StgOpenStorage(
        filePath.c_str(),
        nullptr,
        STGM_READ | STGM_SHARE_DENY_WRITE,
        nullptr,
        0,
        &pStorage
    );
    
    if (FAILED(hr) || !pStorage) {
        LogError(L"MSG 파일 열기 실패: " + filePath);
        return false;
    }
    
    // 루트 스토리지부터 재귀적 탐색 시작
    TraverseStorage(pStorage, L"", outputDir);
    
    // 스토리지 해제
    pStorage->Release();
    
    LogMessage(L"파싱 완료 - 처리된 파일: " + std::to_wstring(m_fileCount) + 
               L", 오류: " + std::to_wstring(m_errorCount));
    
    return m_errorCount == 0;
}

void MSGParser::TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir) {
    if (!pStorage) return;
    
    // 현재 스토리지의 모든 요소 열거
    IEnumSTATSTG* pEnum = nullptr;
    HRESULT hr = pStorage->EnumElements(0, nullptr, 0, &pEnum);
    
    if (FAILED(hr) || !pEnum) {
        LogError(L"스토리지 열거 실패: " + currentPath);
        return;
    }
    
    STATSTG stat;
    ULONG fetched;
    
    while (SUCCEEDED(pEnum->Next(1, &stat, &fetched)) && fetched > 0) {
        std::wstring elementName = stat.pwcsName;
        std::wstring fullPath = currentPath.empty() ? elementName : currentPath + L"\\" + elementName;
        std::wstring safeName = MakeSafeFileName(elementName);
        std::wstring elementOutputPath = outputDir + L"\\" + safeName;
        
        if (stat.type == STGTY_STORAGE) {
            // 스토리지인 경우 - 폴더 생성 후 재귀 탐색
            LogMessage(L"스토리지 발견: " + fullPath);
            
            if (CreateDirectoryRecursive(elementOutputPath)) {
                IStorage* pSubStorage = nullptr;
                hr = pStorage->OpenStorage(
                    stat.pwcsName,
                    nullptr,
                    STGM_READ | STGM_SHARE_DENY_WRITE,
                    nullptr,
                    0,
                    &pSubStorage
                );
                
                if (SUCCEEDED(hr) && pSubStorage) {
                    TraverseStorage(pSubStorage, fullPath, elementOutputPath);
                    pSubStorage->Release();
                } else {
                    LogError(L"하위 스토리지 열기 실패: " + fullPath);
                    m_errorCount++;
                }
            } else {
                LogError(L"디렉토리 생성 실패: " + elementOutputPath);
                m_errorCount++;
            }
        }
        else if (stat.type == STGTY_STREAM) {
            // 스트림인 경우 - hex 파일로 저장
            LogMessage(L"스트림 발견: " + fullPath);
            
            IStream* pStream = nullptr;
            hr = pStorage->OpenStream(
                stat.pwcsName,
                nullptr,
                STGM_READ | STGM_SHARE_DENY_WRITE,
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
                LogError(L"스트림 열기 실패: " + fullPath);
                m_errorCount++;
            }
        }
        
        // 메모리 해제
        if (stat.pwcsName) {
            CoTaskMemFree(stat.pwcsName);
        }
    }
    
    pEnum->Release();
}

bool MSGParser::SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath) {
    if (!pStream) return false;
    
    // 스트림 크기 확인
    STATSTG stat;
    HRESULT hr = pStream->Stat(&stat, STATFLAG_NONAME);
    if (FAILED(hr)) {
        LogError(L"스트림 크기 확인 실패: " + outputPath);
        return false;
    }
    
    DWORD streamSize = stat.cbSize.LowPart;
    if (streamSize == 0) {
        LogMessage(L"빈 스트림 건너뜀: " + outputPath);
        return true;
    }
    
    // 스트림 데이터 읽기
    std::vector<BYTE> buffer(streamSize);
    ULONG bytesRead = 0;
    
    hr = pStream->Read(buffer.data(), streamSize, &bytesRead);
    if (FAILED(hr) || bytesRead != streamSize) {
        LogError(L"스트림 데이터 읽기 실패: " + outputPath);
        return false;
    }
    
    // hex 문자열로 변환
    std::string hexString = BinaryToHexString(buffer.data(), bytesRead);
    
    // 파일로 저장
    std::ofstream file(outputPath);
    if (!file.is_open()) {
        LogError(L"hex 파일 생성 실패: " + outputPath);
        return false;
    }
    
    file << hexString;
    file.close();
    
    LogMessage(L"hex 파일 저장 완료: " + outputPath + L" (" + std::to_wstring(bytesRead) + L" bytes)");
    return true;
}

std::string MSGParser::BinaryToHexString(const BYTE* data, DWORD size) {
    if (!data || size == 0) return "";
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::uppercase;
    
    for (DWORD i = 0; i < size; i++) {
        if (i > 0 && i % 16 == 0) {
            ss << std::endl;  // 16바이트마다 줄바꿈
        }
        ss << std::setw(2) << static_cast<int>(data[i]);
        
        if (i < size - 1 && (i + 1) % 16 != 0) {
            ss << " ";  // 바이트 사이 공백
        }
    }
    
    return ss.str();
}

std::wstring MSGParser::MakeSafeFileName(const std::wstring& originalName) {
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

bool MSGParser::CreateDirectoryRecursive(const std::wstring& path) {
    if (path.empty()) return false;
    
    // 이미 존재하는지 확인
    DWORD attrs = GetFileAttributes(path.c_str());
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
    return CreateDirectory(path.c_str(), nullptr) == TRUE || 
           GetLastError() == ERROR_ALREADY_EXISTS;
}

void MSGParser::LogMessage(const std::wstring& message) {
    std::wcout << L"[INFO] " << message << std::endl;
}

void MSGParser::LogError(const std::wstring& error) {
    std::wcerr << L"[ERROR] " << error << std::endl;
}