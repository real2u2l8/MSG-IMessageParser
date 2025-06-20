#pragma once
#include "pch.h"

// Outlook Mail .msg 파일 관련 상수 정의
// 여기서 Substg를 확인할 수 있다. by. real.hansy
// https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
// http://www.five-ten-sg.com/libpst/rn01re06.html
// OutLook Mail의 .msg 파일 구조를 참고하여 작성하였다.
// KESM_OutMail.exe에서 이 구조로 .msg 파일을 분석하여 정보를 추출한다.
// 상수 정의 부분

// 기본 Substg 상수들
#define		SUBSTG_SENDER		L"__substg1.0_0C1F"		// PR_SENDER_EMAIL_ADDRESS_W:	발신자의 이메일 주소 (Exchange 서버 관련 상수)
#define		SUBSTG_TO			L"__substg1.0_0E04"		// PR_DISPLAY_TO_W:				수신자(To) 전체 표시 문자열,	주소가 아닌 이름만
#define		SUBSTG_CC			L"__substg1.0_0E03"		// PR_DISPLAY_CC_W:				참조(CC) 전체 표시 문자열		주소가 아닌 이름만
#define		SUBSTG_BCC			L"__substg1.0_0E02"		// PR_DISPLAY_BCC_W:			숨은참조(BCC) 전체 표시 문자열	주소가 아닌 이름만
#define		SUBSTG_SUBJECT		L"__substg1.0_0037"		// PR_SUBJECT_W:				메일 제목
#define		SUBSTG_BODY			L"__substg1.0_1000"		// PR_BODY_W:					메일 내용 (텍스트)
#define		SUBSTG_HEADER		L"__substg1.0_007D"		// PR_SUBJECT_PREFIX:			메일 제목 앞 접두사 ("RE:", "FW:" 등)

// 메인 디렉토리들
#define     MAIL_MAIN			L"__nameid_version"		// Named Property의 이름-GUID-Tag 매핑 정보를 저장하는 메인 디렉토리
#define     MAIL_SUBTO			L"__recip_version"		// 각 수신자(TO/CC/BCC)의 Recipient Table 디렉토리
#define		MAIL_PROPERTY		L"__properties_version"	// 메일 디렉토리의 모든 속성 정보를 저장하는 디렉토리
#define     MAIL_ATTACH			L"__attach_version"		// 첨부파일의 모든 속성 정보를 저장하는 Attach 디렉토리

// __attach_version 하위 트리
#define		SUBSTG_ATTACHDISPLAY	L"__substg1.0_3001"		// PR_DISPLAY_NAME_W:        첨부파일 표시 이름
#define		SUBSTG_ATTACHNAME		L"__substg1.0_3707"		// PR_ATTACH_LONG_FILENAME_W: 첨부파일 실제 이름 (전체 경로)
#define		SUBSTG_ATTACHDATA		L"__substg1.0_3701"		// PR_ATTACH_DATA_BIN:        첨부파일 데이터를 저장하는

// 특별 상수들
#define		SUBSTG_ADDRTYPE		    L"__substg1.0_3002"		// PR_ADDRTYPE_W:             주소 타입 (SMTP, EX 등)
#define		SUBSTG_SENDER_ADDRTYPE	L"__substg1.0_0C1E"		// PR_SENDER_ADDRTYPE_W:      발신자 주소 타입 (SMTP, EX 등)
#define		SUBSTG_SMTPADDRESS		L"__substg1.0_39FE"		// PR_SMTP_ADDRESS_W:         EX 타입의 실제 이메일 주소
#define		SUBSTG_SUBTO		    L"__substg1.0_3003"		// PR_EMAIL_ADDRESS_W:        수신자 이메일 주소
#define		SUBSTG_RECIPIENT_TYPE	L"__substg1.0_0C15"		// PR_RECIPIENT_TYPE:		수신자 구분 정보 (To, Cc, Bcc)

// _recip_version 하위 상수
#define		RECIP_TYPE_TO			1       // 수신자(To)
#define		RECIP_TYPE_CC			2       // 참조(Cc)
#define		RECIP_TYPE_BCC			3       // 숨은참조(Bcc)

#define		MAX_RECIPIENT_ADDR_LEN	256  // TO/CC/BCC 버퍼길이

// 250617.Outmail.exe 파일 분석 추가. by. real.hansy
// 이메일 주소 타입을 구분하는 열거형
// SMTP: 일반적인 인터넷 이메일 주소 형식 (예: user@domain.com)
// EX: Exchange 서버에서 사용하는 서버 주소 형식
// UNKNOWN: 알 수 없는 주소 형식
enum class AddrType
{
	SMTP,
	EX,
	UNKNOWN
};

// 250618.Outmail.exe 파일 분석 추가. by. real.hansy
/**
 * @brief 수신자 주소 캐시 구조체
 *
 * @details
 * - 수신자 정보(TO, CC, BCC)를 임시로 저장하기 위한 구조체입니다.
 * - 파싱 과정에서 추출된 문자열을 타입별로 분류하여 저장하고,
 * - 최종적으로 메인 함수에서 mail_info 구조체로 변환하여 사용합니다.
 *
 * @note
 * - 수신자 구분을 정확히 하기 위해 TO/CC/BCC를 개별적으로 분리했으며,
 * - 타입별로 구분하여 저장함으로써 후속 처리에서 로그 및 추가 활용이 가능합니다.
 * - STL map 대신 C 스타일 구조체를 사용하여 처리 속도를 최적화했습니다.
 */
typedef struct _RecipCache {
	char* pszTo;		// To 주소 문자열 (메모리/동적할당으로 관리해야 함)
	char* pszCc;		// Cc 주소 문자열 (메모리/동적할당으로 관리해야 함)
	char* pszBcc;	    // Bcc 주소 문자열 (메모리/동적할당으로 관리해야 함)

	int   nToType;   // 1 = TO
	int   nCcType;   // 2 = CC
	int   nBccType;  // 3 = BCC
} RecipCache;

// MSG 파일 파싱 관련 함수 선언들
class MSGParser {
public:
    MSGParser();
    ~MSGParser();
    
    // 메인 파싱 함수
    bool ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir);
    
private:
    // 재귀적 스토리지/스트림 탐색
    void TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir);
    
    // 스트림 데이터를 hex로 변환하여 파일로 저장
    bool SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath);
    
    // 바이너리 데이터를 hex 문자열로 변환
    std::string BinaryToHexString(const BYTE* data, DWORD size);
    
    // 안전한 파일명 생성 (특수문자 처리)
    std::wstring MakeSafeFileName(const std::wstring& originalName);
    
    // 디렉토리 생성
    bool CreateDirectoryRecursive(const std::wstring& path);
    
    // 유틸리티 함수들
    void LogMessage(const std::wstring& message);
    void LogError(const std::wstring& error);
    
private:
    std::wstring m_outputBaseDir;
    int m_fileCount;
    int m_errorCount;
};