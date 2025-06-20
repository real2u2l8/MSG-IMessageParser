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


// MSG 파일 파싱 관련 함수 선언들
/**
 * @brief MSG 파일 파싱을 담당하는 메인 클래스
 *
 * @details
 * 이 클래스는 Outlook .msg 파일의 구조를 분석하고 내부 데이터를 추출하는 기능을 제공합니다.
 * IStorage 인터페이스를 사용하여 MSG 파일의 계층적 구조를 탐색하고,
 * 각 스트림의 데이터를 hex 형태로 저장합니다.
 *
 * @note
 * - COM 초기화/해제를 자동으로 처리합니다.
 * - 재귀적 탐색을 통해 모든 하위 스토리지와 스트림을 처리합니다.
 * - 안전한 파일명 생성 및 디렉토리 생성을 지원합니다.
 *
 * @see https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
 */
class MSGParser {
public:
    /**
     * @brief MSGParser 생성자
     *
     * @details
     * MSGParser 객체를 초기화하고 COM 라이브러리를 초기화합니다.
     * 파일 카운터와 에러 카운터를 0으로 설정합니다.
     */
    MSGParser();
    
    /**
     * @brief MSGParser 소멸자
     *
     * @details
     * 모든 리소스를 정리하고 COM 라이브러리를 해제합니다.
     * 정리 과정을 로그로 기록합니다.
     */
    ~MSGParser();
    
    /**
     * @brief MSG 파일을 파싱하여 출력 디렉토리에 구조를 저장
     *
     * @param[in] filePath 파싱할 MSG 파일의 경로
     * @param[in] outputDir 결과를 저장할 출력 디렉토리 경로
     * @return true 파싱 성공, false 파싱 실패
     *
     * @details
     * 지정된 MSG 파일을 열어서 내부 구조를 분석합니다.
     * 모든 스토리지와 스트림을 재귀적으로 탐색하여 출력 디렉토리에 저장합니다.
     * 각 스트림은 hex 파일 형태로 저장됩니다.
     *
     * @note
     * - 출력 디렉토리가 존재하지 않으면 자동으로 생성합니다.
     * - 파일 접근 권한 문제가 있을 경우 다양한 접근 모드로 재시도합니다.
     * - 처리된 파일 수와 에러 수를 내부적으로 추적합니다.
     */
    bool ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir);
    
private:
    /**
     * @brief 스토리지를 재귀적으로 탐색하여 모든 요소를 처리
     *
     * @param[in] pStorage 탐색할 IStorage 포인터
     * @param[in] currentPath 현재 탐색 중인 경로 (디버깅용)
     * @param[in] outputDir 현재 레벨의 출력 디렉토리
     *
     * @details
     * IStorage의 모든 요소(스토리지와 스트림)를 열거하여 처리합니다.
     * 스토리지인 경우 하위 디렉토리를 생성하고 재귀적으로 탐색합니다.
     * 스트림인 경우 hex 파일로 저장합니다.
     *
     * @note
     * - 메모리 누수를 방지하기 위해 STATSTG 구조체의 pwcsName을 적절히 해제합니다.
     * - 접근 권한 문제가 있을 경우 다양한 접근 모드로 재시도합니다.
     */
    void TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir);
    
    /**
     * @brief 스트림 데이터를 hex 형태로 변환하여 파일에 저장
     *
     * @param[in] pStream 저장할 IStream 포인터
     * @param[in] outputPath 저장할 파일 경로 (.hex 확장자 포함)
     * @return true 저장 성공, false 저장 실패
     *
     * @details
     * IStream의 모든 데이터를 읽어서 hex 문자열로 변환한 후 파일에 저장합니다.
     * 16바이트마다 줄바꿈을 추가하여 가독성을 높입니다.
     *
     * @note
     * - 빈 스트림은 건너뜁니다.
     * - 스트림 크기가 0인 경우 성공으로 처리합니다.
     */
    bool SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath);
    
    /**
     * @brief 바이너리 데이터를 hex 문자열로 변환
     *
     * @param[in] data 변환할 바이너리 데이터 포인터
     * @param[in] size 데이터 크기 (바이트)
     * @return hex 문자열로 변환된 데이터
     *
     * @details
     * 바이너리 데이터를 16진수 문자열로 변환합니다.
     * 16바이트마다 줄바꿈을 추가하고, 바이트 사이에 공백을 삽입합니다.
     * 대문자 hex 형식을 사용합니다.
     *
     * @example
     * @code
     * BYTE data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
     * std::string hex = BinaryToHexString(data, 5);
     * // 결과: "48 65 6C 6C 6F"
     * @endcode
     */
    std::string BinaryToHexString(const BYTE* data, DWORD size);
    
    /**
     * @brief 안전한 파일명 생성 (특수문자 처리)
     *
     * @param[in] originalName 원본 파일명
     * @return 안전한 파일명
     *
     * @details
     * Windows 파일명에서 사용할 수 없는 특수문자들을 언더스코어로 변경합니다.
     * 파일명이 너무 길 경우 200자로 자릅니다.
     *
     * @note
     * 변경되는 문자: < > : " / \ | ? *
     */
    std::wstring MakeSafeFileName(const std::wstring& originalName);
    
    /**
     * @brief 디렉토리를 재귀적으로 생성
     *
     * @param[in] path 생성할 디렉토리 경로
     * @return true 생성 성공 또는 이미 존재, false 생성 실패
     *
     * @details
     * 지정된 경로의 모든 상위 디렉토리를 포함하여 디렉토리를 생성합니다.
     * 이미 존재하는 디렉토리는 성공으로 처리합니다.
     *
     * @note
     * - Windows API의 CreateDirectoryW를 사용합니다.
     * - ERROR_ALREADY_EXISTS 에러는 성공으로 처리합니다.
     */
    bool CreateDirectoryRecursive(const std::wstring& path);
    
    /**
     * @brief 정보 메시지를 콘솔에 출력
     *
     * @param[in] message 출력할 메시지
     *
     * @details
     * [INFO] 접두사와 함께 메시지를 표준 출력에 출력합니다.
     */
    void LogMessage(const std::wstring& message);
    
    /**
     * @brief 에러 메시지를 콘솔에 출력
     *
     * @param[in] error 출력할 에러 메시지
     *
     * @details
     * [ERROR] 접두사와 함께 메시지를 표준 에러 출력에 출력합니다.
     */
    void LogError(const std::wstring& error);
    
private:
    std::wstring m_outputBaseDir;  ///< 출력 기본 디렉토리 경로
    int m_fileCount;               ///< 처리된 파일 수
    int m_errorCount;              ///< 발생한 에러 수
};

// MAPI Property Types
#define PT_UNSPECIFIED     0x0000
#define PT_NULL            0x0001
#define PT_I2              0x0002  // 16-bit integer
#define PT_LONG            0x0003  // 32-bit integer
#define PT_R4              0x0004  // 4-byte floating point
#define PT_DOUBLE          0x0005  // 8-byte floating point
#define PT_CURRENCY        0x0006  // 8-byte currency
#define PT_APPTIME         0x0007  // 8-byte application time
#define PT_ERROR           0x000A  // 32-bit error code
#define PT_BOOLEAN         0x000B  // 16-bit boolean
#define PT_OBJECT          0x000D  // Embedded object
#define PT_I8              0x0014  // 8-byte integer
#define PT_STRING8         0x001E  // 8-bit character string
#define PT_UNICODE         0x001F  // Unicode string
#define PT_SYSTIME         0x0040  // 8-byte system time
#define PT_CLSID           0x0048  // 16-byte GUID
#define PT_BINARY          0x0102  // Binary data
#define PT_MV_I2           0x1002  // Multi-value 16-bit integer
#define PT_MV_LONG         0x1003  // Multi-value 32-bit integer
#define PT_MV_R4           0x1004  // Multi-value 4-byte float
#define PT_MV_DOUBLE       0x1005  // Multi-value 8-byte float
#define PT_MV_CURRENCY     0x1006  // Multi-value currency
#define PT_MV_APPTIME      0x1007  // Multi-value application time
#define PT_MV_SYSTIME      0x1040  // Multi-value system time
#define PT_MV_STRING8      0x101E  // Multi-value 8-bit string
#define PT_MV_UNICODE      0x101F  // Multi-value Unicode string
#define PT_MV_CLSID        0x1048  // Multi-value GUID
#define PT_MV_BINARY       0x1102  // Multi-value binary

// MAPI Property Tags (주요 태그들)
#define PR_RECIPIENT_TYPE          0x0C15  // 수신자 타입 (To, Cc, Bcc)
#define PR_DISPLAY_NAME            0x3001  // 표시 이름
#define PR_SMTP_ADDRESS            0x39FE  // SMTP 주소
#define PR_EMAIL_ADDRESS           0x3003  // 이메일 주소
#define PR_ADDRTYPE                0x3002  // 주소 타입
#define PR_ENTRYID                 0x0FFF  // Entry ID
#define PR_SEARCH_KEY              0x300B  // 검색 키
#define PR_TRANSMITABLE_DISPLAY_NAME 0x3001  // 전송 가능한 표시 이름

/**
 * @brief MAPI Property 정의 구조체
 *
 * @details
 * MAPI Property Stream에서 각 속성의 정의 정보를 저장하는 구조체입니다.
 * 16바이트 단위로 구성되며, Property Tag, Type, Offset, Size 정보를 포함합니다.
 *
 * @note
 * 이 구조체는 MSG 파일의 __properties_version 스트림에서 사용됩니다.
 */
typedef struct _PROPERTY_DEFINITION {
    WORD    PropertyTag;      ///< Property Tag (2 bytes) - 속성 ID와 타입을 조합한 값
    WORD    PropertyType;     ///< Property Type (2 bytes) - 데이터 타입 (PT_STRING8, PT_UNICODE 등)
    DWORD   Offset;           ///< Value offset from start of value section (4 bytes) - 값 섹션 내 오프셋
    DWORD   Size;             ///< Value size in bytes (4 bytes) - 값의 크기
} PROPERTY_DEFINITION, *PPROPERTY_DEFINITION;

/**
 * @brief MAPI Property 값 구조체
 *
 * @details
 * 파싱된 MAPI Property의 실제 값을 저장하는 구조체입니다.
 * Property Tag, Type, Size 정보와 함께 실제 데이터 포인터를 포함합니다.
 *
 * @note
 * Data 포인터는 동적으로 할당되므로 사용 후 반드시 해제해야 합니다.
 */
typedef struct _PROPERTY_VALUE {
    WORD    PropertyTag;      ///< Property Tag - 속성 ID
    WORD    PropertyType;     ///< Property Type - 데이터 타입
    DWORD   Size;             ///< Value size - 데이터 크기
    BYTE*   Data;             ///< Value data pointer - 실제 데이터 포인터
} PROPERTY_VALUE, *PPROPERTY_VALUE;

/**
 * @brief RAII 파일 핸들 래퍼 클래스
 *
 * @details
 * 파일 핸들의 자동 관리를 위한 RAII(Resource Acquisition Is Initialization) 패턴을 구현한 클래스입니다.
 * 생성자에서 파일을 열고, 소멸자에서 자동으로 파일을 닫습니다.
 *
 * @note
 * - 예외 안전성을 보장합니다.
 * - 파일 핸들 누수를 방지합니다.
 * - std::ifstream을 내부적으로 사용합니다.
 */
class SafeFileHandle {
public:
    /**
     * @brief SafeFileHandle 생성자
     *
     * @param[in] path 열 파일 경로
     * @param[in] mode 파일 열기 모드 (기본값: std::ios::in)
     *
     * @details
     * 지정된 경로의 파일을 지정된 모드로 엽니다.
     * 파일 열기에 실패해도 예외를 발생시키지 않습니다.
     */
    SafeFileHandle(const std::wstring& path, std::ios::openmode mode = std::ios::in);
    
    /**
     * @brief SafeFileHandle 소멸자
     *
     * @details
     * 파일이 열려있는 경우 자동으로 닫습니다.
     */
    ~SafeFileHandle();
    
    /**
     * @brief 파일이 열려있는지 확인
     *
     * @return true 파일이 열려있음, false 파일이 닫혀있음
     */
    bool is_open() const;
    
    /**
     * @brief 내부 파일 스트림 참조 반환
     *
     * @return std::ifstream 참조
     *
     * @details
     * 내부 std::ifstream 객체에 대한 참조를 반환합니다.
     * 파일 읽기 작업에 사용할 수 있습니다.
     */
    std::ifstream& get();
    
private:
    std::wstring m_path;          ///< 파일 경로
    std::ios::openmode m_mode;    ///< 파일 열기 모드
    std::ifstream m_file;         ///< 내부 파일 스트림 객체
};

/**
 * @brief MAPI Property Stream 파싱을 담당하는 클래스
 *
 * @details
 * MSG 파일의 MAPI Property Stream을 파싱하여 개별 속성들을 추출하고 해석하는 클래스입니다.
 * 16바이트 단위의 Property Definition을 분석하고, Value Section에서 실제 데이터를 추출합니다.
 * 다양한 데이터 타입(String8, Unicode, Long, Binary 등)을 지원합니다.
 *
 * @note
 * - 디버그 로깅 기능을 포함합니다.
 * - 메모리 관리를 자동으로 처리합니다.
 * - 다양한 MAPI Property Tag와 Type을 지원합니다.
 *
 * @see https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
 */
class MAPIPropertyParser {
public:
    /**
     * @brief MAPIPropertyParser 생성자
     *
     * @details
     * MAPIPropertyParser 객체를 초기화합니다.
     * 디버그 로깅을 활성화하고 로그 레벨을 3(DEBUG)으로 설정합니다.
     */
    MAPIPropertyParser();
    
    /**
     * @brief MAPIPropertyParser 소멸자
     *
     * @details
     * 모든 동적으로 할당된 Property Value 데이터를 해제합니다.
     * 정리 과정을 디버그 로그로 기록합니다.
     */
    ~MAPIPropertyParser();
    
    /**
     * @brief MAPI Property Stream 데이터를 파싱
     *
     * @param[in] streamData 파싱할 스트림 데이터
     * @return true 파싱 성공, false 파싱 실패
     *
     * @details
     * MAPI Property Stream의 바이너리 데이터를 분석하여 개별 속성들을 추출합니다.
     * 16바이트 단위로 Property Definition을 읽고, Value Section에서 실제 데이터를 추출합니다.
     *
     * @note
     * - 최소 16바이트의 데이터가 필요합니다.
     * - Property Definition Section과 Value Section을 구분하여 처리합니다.
     * - 각 속성의 타입에 따라 적절한 해석을 수행합니다.
     */
    bool ParsePropertiesStream(const std::vector<BYTE>& streamData);
    
    /**
     * @brief 지정된 Property Tag의 문자열 값 반환
     *
     * @param[in] propertyTag 찾을 Property Tag
     * @return 문자열 값 (찾지 못한 경우 빈 문자열)
     *
     * @details
     * 지정된 Property Tag에 해당하는 속성을 찾아서 문자열로 해석하여 반환합니다.
     * 데이터 타입에 따라 적절한 변환을 수행합니다.
     */
    std::wstring GetStringValue(WORD propertyTag);
    
    /**
     * @brief 지정된 Property Tag의 32비트 정수 값 반환
     *
     * @param[in] propertyTag 찾을 Property Tag
     * @return 32비트 정수 값 (찾지 못한 경우 0)
     *
     * @details
     * 지정된 Property Tag에 해당하는 속성을 찾아서 32비트 정수로 해석하여 반환합니다.
     * 최소 4바이트의 데이터가 있어야 합니다.
     */
    DWORD GetLongValue(WORD propertyTag);
    
    /**
     * @brief 지정된 Property Tag의 바이너리 데이터 반환
     *
     * @param[in] propertyTag 찾을 Property Tag
     * @return 바이너리 데이터 벡터 (찾지 못한 경우 빈 벡터)
     *
     * @details
     * 지정된 Property Tag에 해당하는 속성을 찾아서 바이너리 데이터로 반환합니다.
     * std::vector<BYTE> 형태로 반환됩니다.
     */
    std::vector<BYTE> GetBinaryValue(WORD propertyTag);
    
    /**
     * @brief 파싱된 모든 속성을 콘솔에 출력
     *
     * @details
     * 파싱된 모든 MAPI Property를 콘솔에 출력합니다.
     * 각 속성의 Tag, Type, Size, Value 정보를 표시합니다.
     */
    void PrintParsedProperties();
    
    /**
     * @brief hex 파일에서 데이터를 읽어서 파싱
     *
     * @param[in] hexFilePath 읽을 hex 파일 경로
     * @return true 파싱 성공, false 파싱 실패
     *
     * @details
     * hex 파일의 각 줄을 읽어서 바이너리 데이터로 변환한 후 파싱을 수행합니다.
     * 2자리 hex 값을 바이트로 변환하여 처리합니다.
     *
     * @note
     * - 공백 문자는 자동으로 제거됩니다.
     * - 잘못된 hex 값은 경고 로그로 기록하고 건너뜁니다.
     */
    bool ParseFromHexFile(const std::wstring& hexFilePath);
    
    /**
     * @brief 디버그 로깅 활성화/비활성화
     *
     * @param[in] enable true: 활성화, false: 비활성화
     */
    void EnableDebugLog(bool enable);
    
    /**
     * @brief 로그 레벨 설정
     *
     * @param[in] level 로그 레벨 (0=Error, 1=Warning, 2=Info, 3=Debug)
     */
    void SetLogLevel(int level);
    
private:
    /**
     * @brief Property Value를 문자열로 해석
     *
     * @param[in] value 해석할 Property Value
     * @return 해석된 문자열
     *
     * @details
     * Property Type에 따라 적절한 해석 방법을 선택하여 문자열로 변환합니다.
     * 지원하는 타입: PT_STRING8, PT_UNICODE, PT_LONG, PT_BINARY, PT_BOOLEAN
     */
    std::wstring InterpretValue(const PROPERTY_VALUE& value);
    
    /**
     * @brief 8비트 문자열 데이터를 유니코드로 변환
     *
     * @param[in] data 8비트 문자열 데이터
     * @param[in] size 데이터 크기
     * @return 변환된 유니코드 문자열
     *
     * @details
     * 8비트 문자열(ANSI)을 유니코드 문자열로 변환합니다.
     * Windows API의 MultiByteToWideChar를 사용합니다.
     */
    std::wstring InterpretString8(const BYTE* data, DWORD size);
    
    /**
     * @brief 유니코드 문자열 데이터를 해석
     *
     * @param[in] data 유니코드 문자열 데이터
     * @param[in] size 데이터 크기
     * @return 해석된 유니코드 문자열
     *
     * @details
     * 2바이트씩 구성된 유니코드 문자열을 해석합니다.
     * size/2 개의 문자로 변환합니다.
     */
    std::wstring InterpretUnicode(const BYTE* data, DWORD size);
    
    /**
     * @brief 바이너리 데이터를 hex 문자열로 변환
     *
     * @param[in] data 바이너리 데이터
     * @param[in] size 데이터 크기
     * @return hex 문자열로 변환된 데이터
     *
     * @details
     * 바이너리 데이터를 hex 문자열로 변환합니다.
     * 최대 16바이트까지만 표시하고, 나머지는 "..."으로 표시합니다.
     */
    std::wstring InterpretBinary(const BYTE* data, DWORD size);
    
    /**
     * @brief 32비트 정수 데이터를 해석
     *
     * @param[in] data 32비트 정수 데이터
     * @param[in] size 데이터 크기
     * @return 해석된 정수 문자열
     *
     * @details
     * 최소 4바이트의 데이터를 32비트 정수로 해석하여 문자열로 변환합니다.
     */
    std::wstring InterpretLong(const BYTE* data, DWORD size);
    
    /**
     * @brief Property Tag를 문자열로 변환
     *
     * @param[in] propertyTag 변환할 Property Tag
     * @return Property Tag 문자열
     *
     * @details
     * 알려진 Property Tag들을 문자열로 변환합니다.
     * 알 수 없는 Tag는 "UNKNOWN_TAG"로 반환합니다.
     */
    std::wstring PropertyTagToString(WORD propertyTag);
    
    /**
     * @brief Property Type을 문자열로 변환
     *
     * @param[in] propertyType 변환할 Property Type
     * @return Property Type 문자열
     *
     * @details
     * 알려진 Property Type들을 문자열로 변환합니다.
     * 알 수 없는 Type은 "UNKNOWN_TYPE"으로 반환합니다.
     */
    std::wstring PropertyTypeToString(WORD propertyType);
    
    /**
     * @brief 디버그 레벨 로그 출력
     *
     * @param[in] message 출력할 메시지
     *
     * @details
     * 디버그 로깅이 활성화되어 있고 로그 레벨이 3 이상일 때만 출력합니다.
     */
    void LogDebug(const std::wstring& message);
    
    /**
     * @brief 정보 레벨 로그 출력
     *
     * @param[in] message 출력할 메시지
     *
     * @details
     * 디버그 로깅이 활성화되어 있고 로그 레벨이 2 이상일 때만 출력합니다.
     */
    void LogInfo(const std::wstring& message);
    
    /**
     * @brief 경고 레벨 로그 출력
     *
     * @param[in] message 출력할 메시지
     *
     * @details
     * 디버그 로깅이 활성화되어 있고 로그 레벨이 1 이상일 때만 출력합니다.
     */
    void LogWarning(const std::wstring& message);
    
    /**
     * @brief 에러 레벨 로그 출력
     *
     * @param[in] message 출력할 메시지
     *
     * @details
     * 디버그 로깅이 활성화되어 있고 로그 레벨이 0 이상일 때만 출력합니다.
     */
    void LogError(const std::wstring& message);
    
    /**
     * @brief 바이너리 데이터를 hex dump 형태로 출력
     *
     * @param[in] prefix 출력할 접두사
     * @param[in] data 출력할 바이너리 데이터
     * @param[in] size 데이터 크기
     * @param[in] maxBytes 최대 출력할 바이트 수 (기본값: 32)
     *
     * @details
     * 바이너리 데이터를 hex 형태로 출력합니다.
     * 디버그 로깅이 활성화되어 있고 로그 레벨이 3 이상일 때만 출력합니다.
     */
    void LogHexDump(const std::wstring& prefix, const BYTE* data, DWORD size, DWORD maxBytes = 32);
    
private:
    std::vector<PROPERTY_VALUE> m_properties;  ///< 파싱된 속성들의 벡터
    bool m_debugEnabled;                       ///< 디버그 로깅 활성화 여부
    int m_logLevel;                            ///< 로그 레벨 (0=Error, 1=Warning, 2=Info, 3=Debug)
};