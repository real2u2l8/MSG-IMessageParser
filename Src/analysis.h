#pragma once
#include "pch.h"

// Outlook Mail .msg ���� ���� ��� ����
// ���⼭ Substg�� Ȯ���� �� �ִ�. by. real.hansy
// https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
// http://www.five-ten-sg.com/libpst/rn01re06.html
// OutLook Mail�� .msg ���� ������ �����Ͽ� �ۼ��Ͽ���.
// KESM_OutMail.exe���� �� ������ .msg ������ �м��Ͽ� ������ �����Ѵ�.
// ��� ���� �κ�

// �⺻ Substg �����
#define		SUBSTG_SENDER		L"__substg1.0_0C1F"		// PR_SENDER_EMAIL_ADDRESS_W:	�߽����� �̸��� �ּ� (Exchange ���� ���� ���)
#define		SUBSTG_TO			L"__substg1.0_0E04"		// PR_DISPLAY_TO_W:				������(To) ��ü ǥ�� ���ڿ�,	�ּҰ� �ƴ� �̸���
#define		SUBSTG_CC			L"__substg1.0_0E03"		// PR_DISPLAY_CC_W:				����(CC) ��ü ǥ�� ���ڿ�		�ּҰ� �ƴ� �̸���
#define		SUBSTG_BCC			L"__substg1.0_0E02"		// PR_DISPLAY_BCC_W:			��������(BCC) ��ü ǥ�� ���ڿ�	�ּҰ� �ƴ� �̸���
#define		SUBSTG_SUBJECT		L"__substg1.0_0037"		// PR_SUBJECT_W:				���� ����
#define		SUBSTG_BODY			L"__substg1.0_1000"		// PR_BODY_W:					���� ���� (�ؽ�Ʈ)
#define		SUBSTG_HEADER		L"__substg1.0_007D"		// PR_SUBJECT_PREFIX:			���� ���� �� ���λ� ("RE:", "FW:" ��)

// ���� ���丮��
#define     MAIL_MAIN			L"__nameid_version"		// Named Property�� �̸�-GUID-Tag ���� ������ �����ϴ� ���� ���丮
#define     MAIL_SUBTO			L"__recip_version"		// �� ������(TO/CC/BCC)�� Recipient Table ���丮
#define		MAIL_PROPERTY		L"__properties_version"	// ���� ���丮�� ��� �Ӽ� ������ �����ϴ� ���丮
#define     MAIL_ATTACH			L"__attach_version"		// ÷�������� ��� �Ӽ� ������ �����ϴ� Attach ���丮

// __attach_version ���� Ʈ��
#define		SUBSTG_ATTACHDISPLAY	L"__substg1.0_3001"		// PR_DISPLAY_NAME_W:        ÷������ ǥ�� �̸�
#define		SUBSTG_ATTACHNAME		L"__substg1.0_3707"		// PR_ATTACH_LONG_FILENAME_W: ÷������ ���� �̸� (��ü ���)
#define		SUBSTG_ATTACHDATA		L"__substg1.0_3701"		// PR_ATTACH_DATA_BIN:        ÷������ �����͸� �����ϴ�

// Ư�� �����
#define		SUBSTG_ADDRTYPE		    L"__substg1.0_3002"		// PR_ADDRTYPE_W:             �ּ� Ÿ�� (SMTP, EX ��)
#define		SUBSTG_SENDER_ADDRTYPE	L"__substg1.0_0C1E"		// PR_SENDER_ADDRTYPE_W:      �߽��� �ּ� Ÿ�� (SMTP, EX ��)
#define		SUBSTG_SMTPADDRESS		L"__substg1.0_39FE"		// PR_SMTP_ADDRESS_W:         EX Ÿ���� ���� �̸��� �ּ�
#define		SUBSTG_SUBTO		    L"__substg1.0_3003"		// PR_EMAIL_ADDRESS_W:        ������ �̸��� �ּ�
#define		SUBSTG_RECIPIENT_TYPE	L"__substg1.0_0C15"		// PR_RECIPIENT_TYPE:		������ ���� ���� (To, Cc, Bcc)

// _recip_version ���� ���
#define		RECIP_TYPE_TO			1       // ������(To)
#define		RECIP_TYPE_CC			2       // ����(Cc)
#define		RECIP_TYPE_BCC			3       // ��������(Bcc)

#define		MAX_RECIPIENT_ADDR_LEN	256  // TO/CC/BCC ���۱���


// MSG ���� �Ľ� ���� �Լ� �����
/**
 * @brief MSG ���� �Ľ��� ����ϴ� ���� Ŭ����
 *
 * @details
 * �� Ŭ������ Outlook .msg ������ ������ �м��ϰ� ���� �����͸� �����ϴ� ����� �����մϴ�.
 * IStorage �������̽��� ����Ͽ� MSG ������ ������ ������ Ž���ϰ�,
 * �� ��Ʈ���� �����͸� hex ���·� �����մϴ�.
 *
 * @note
 * - COM �ʱ�ȭ/������ �ڵ����� ó���մϴ�.
 * - ����� Ž���� ���� ��� ���� ���丮���� ��Ʈ���� ó���մϴ�.
 * - ������ ���ϸ� ���� �� ���丮 ������ �����մϴ�.
 *
 * @see https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
 */
class MSGParser {
public:
    /**
     * @brief MSGParser ������
     *
     * @details
     * MSGParser ��ü�� �ʱ�ȭ�ϰ� COM ���̺귯���� �ʱ�ȭ�մϴ�.
     * ���� ī���Ϳ� ���� ī���͸� 0���� �����մϴ�.
     */
    MSGParser();
    
    /**
     * @brief MSGParser �Ҹ���
     *
     * @details
     * ��� ���ҽ��� �����ϰ� COM ���̺귯���� �����մϴ�.
     * ���� ������ �α׷� ����մϴ�.
     */
    ~MSGParser();
    
    /**
     * @brief MSG ������ �Ľ��Ͽ� ��� ���丮�� ������ ����
     *
     * @param[in] filePath �Ľ��� MSG ������ ���
     * @param[in] outputDir ����� ������ ��� ���丮 ���
     * @return true �Ľ� ����, false �Ľ� ����
     *
     * @details
     * ������ MSG ������ ��� ���� ������ �м��մϴ�.
     * ��� ���丮���� ��Ʈ���� ��������� Ž���Ͽ� ��� ���丮�� �����մϴ�.
     * �� ��Ʈ���� hex ���� ���·� ����˴ϴ�.
     *
     * @note
     * - ��� ���丮�� �������� ������ �ڵ����� �����մϴ�.
     * - ���� ���� ���� ������ ���� ��� �پ��� ���� ���� ��õ��մϴ�.
     * - ó���� ���� ���� ���� ���� ���������� �����մϴ�.
     */
    bool ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir);
    
private:
    /**
     * @brief ���丮���� ��������� Ž���Ͽ� ��� ��Ҹ� ó��
     *
     * @param[in] pStorage Ž���� IStorage ������
     * @param[in] currentPath ���� Ž�� ���� ��� (������)
     * @param[in] outputDir ���� ������ ��� ���丮
     *
     * @details
     * IStorage�� ��� ���(���丮���� ��Ʈ��)�� �����Ͽ� ó���մϴ�.
     * ���丮���� ��� ���� ���丮�� �����ϰ� ��������� Ž���մϴ�.
     * ��Ʈ���� ��� hex ���Ϸ� �����մϴ�.
     *
     * @note
     * - �޸� ������ �����ϱ� ���� STATSTG ����ü�� pwcsName�� ������ �����մϴ�.
     * - ���� ���� ������ ���� ��� �پ��� ���� ���� ��õ��մϴ�.
     */
    void TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir);
    
    /**
     * @brief ��Ʈ�� �����͸� hex ���·� ��ȯ�Ͽ� ���Ͽ� ����
     *
     * @param[in] pStream ������ IStream ������
     * @param[in] outputPath ������ ���� ��� (.hex Ȯ���� ����)
     * @return true ���� ����, false ���� ����
     *
     * @details
     * IStream�� ��� �����͸� �о hex ���ڿ��� ��ȯ�� �� ���Ͽ� �����մϴ�.
     * 16����Ʈ���� �ٹٲ��� �߰��Ͽ� �������� ���Դϴ�.
     *
     * @note
     * - �� ��Ʈ���� �ǳʶݴϴ�.
     * - ��Ʈ�� ũ�Ⱑ 0�� ��� �������� ó���մϴ�.
     */
    bool SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath);
    
    /**
     * @brief ���̳ʸ� �����͸� hex ���ڿ��� ��ȯ
     *
     * @param[in] data ��ȯ�� ���̳ʸ� ������ ������
     * @param[in] size ������ ũ�� (����Ʈ)
     * @return hex ���ڿ��� ��ȯ�� ������
     *
     * @details
     * ���̳ʸ� �����͸� 16���� ���ڿ��� ��ȯ�մϴ�.
     * 16����Ʈ���� �ٹٲ��� �߰��ϰ�, ����Ʈ ���̿� ������ �����մϴ�.
     * �빮�� hex ������ ����մϴ�.
     *
     * @example
     * @code
     * BYTE data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
     * std::string hex = BinaryToHexString(data, 5);
     * // ���: "48 65 6C 6C 6F"
     * @endcode
     */
    std::string BinaryToHexString(const BYTE* data, DWORD size);
    
    /**
     * @brief ������ ���ϸ� ���� (Ư������ ó��)
     *
     * @param[in] originalName ���� ���ϸ�
     * @return ������ ���ϸ�
     *
     * @details
     * Windows ���ϸ��� ����� �� ���� Ư�����ڵ��� ������ھ�� �����մϴ�.
     * ���ϸ��� �ʹ� �� ��� 200�ڷ� �ڸ��ϴ�.
     *
     * @note
     * ����Ǵ� ����: < > : " / \ | ? *
     */
    std::wstring MakeSafeFileName(const std::wstring& originalName);
    
    /**
     * @brief ���丮�� ��������� ����
     *
     * @param[in] path ������ ���丮 ���
     * @return true ���� ���� �Ǵ� �̹� ����, false ���� ����
     *
     * @details
     * ������ ����� ��� ���� ���丮�� �����Ͽ� ���丮�� �����մϴ�.
     * �̹� �����ϴ� ���丮�� �������� ó���մϴ�.
     *
     * @note
     * - Windows API�� CreateDirectoryW�� ����մϴ�.
     * - ERROR_ALREADY_EXISTS ������ �������� ó���մϴ�.
     */
    bool CreateDirectoryRecursive(const std::wstring& path);
    
    /**
     * @brief ���� �޽����� �ֿܼ� ���
     *
     * @param[in] message ����� �޽���
     *
     * @details
     * [INFO] ���λ�� �Բ� �޽����� ǥ�� ��¿� ����մϴ�.
     */
    void LogMessage(const std::wstring& message);
    
    /**
     * @brief ���� �޽����� �ֿܼ� ���
     *
     * @param[in] error ����� ���� �޽���
     *
     * @details
     * [ERROR] ���λ�� �Բ� �޽����� ǥ�� ���� ��¿� ����մϴ�.
     */
    void LogError(const std::wstring& error);
    
private:
    std::wstring m_outputBaseDir;  ///< ��� �⺻ ���丮 ���
    int m_fileCount;               ///< ó���� ���� ��
    int m_errorCount;              ///< �߻��� ���� ��
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

// MAPI Property Tags (�ֿ� �±׵�)
#define PR_RECIPIENT_TYPE          0x0C15  // ������ Ÿ�� (To, Cc, Bcc)
#define PR_DISPLAY_NAME            0x3001  // ǥ�� �̸�
#define PR_SMTP_ADDRESS            0x39FE  // SMTP �ּ�
#define PR_EMAIL_ADDRESS           0x3003  // �̸��� �ּ�
#define PR_ADDRTYPE                0x3002  // �ּ� Ÿ��
#define PR_ENTRYID                 0x0FFF  // Entry ID
#define PR_SEARCH_KEY              0x300B  // �˻� Ű
#define PR_TRANSMITABLE_DISPLAY_NAME 0x3001  // ���� ������ ǥ�� �̸�

/**
 * @brief MAPI Property ���� ����ü
 *
 * @details
 * MAPI Property Stream���� �� �Ӽ��� ���� ������ �����ϴ� ����ü�Դϴ�.
 * 16����Ʈ ������ �����Ǹ�, Property Tag, Type, Offset, Size ������ �����մϴ�.
 *
 * @note
 * �� ����ü�� MSG ������ __properties_version ��Ʈ������ ���˴ϴ�.
 */
typedef struct _PROPERTY_DEFINITION {
    WORD    PropertyTag;      ///< Property Tag (2 bytes) - �Ӽ� ID�� Ÿ���� ������ ��
    WORD    PropertyType;     ///< Property Type (2 bytes) - ������ Ÿ�� (PT_STRING8, PT_UNICODE ��)
    DWORD   Offset;           ///< Value offset from start of value section (4 bytes) - �� ���� �� ������
    DWORD   Size;             ///< Value size in bytes (4 bytes) - ���� ũ��
} PROPERTY_DEFINITION, *PPROPERTY_DEFINITION;

/**
 * @brief MAPI Property �� ����ü
 *
 * @details
 * �Ľ̵� MAPI Property�� ���� ���� �����ϴ� ����ü�Դϴ�.
 * Property Tag, Type, Size ������ �Բ� ���� ������ �����͸� �����մϴ�.
 *
 * @note
 * Data �����ʹ� �������� �Ҵ�ǹǷ� ��� �� �ݵ�� �����ؾ� �մϴ�.
 */
typedef struct _PROPERTY_VALUE {
    WORD    PropertyTag;      ///< Property Tag - �Ӽ� ID
    WORD    PropertyType;     ///< Property Type - ������ Ÿ��
    DWORD   Size;             ///< Value size - ������ ũ��
    BYTE*   Data;             ///< Value data pointer - ���� ������ ������
} PROPERTY_VALUE, *PPROPERTY_VALUE;

/**
 * @brief RAII ���� �ڵ� ���� Ŭ����
 *
 * @details
 * ���� �ڵ��� �ڵ� ������ ���� RAII(Resource Acquisition Is Initialization) ������ ������ Ŭ�����Դϴ�.
 * �����ڿ��� ������ ����, �Ҹ��ڿ��� �ڵ����� ������ �ݽ��ϴ�.
 *
 * @note
 * - ���� �������� �����մϴ�.
 * - ���� �ڵ� ������ �����մϴ�.
 * - std::ifstream�� ���������� ����մϴ�.
 */
class SafeFileHandle {
public:
    /**
     * @brief SafeFileHandle ������
     *
     * @param[in] path �� ���� ���
     * @param[in] mode ���� ���� ��� (�⺻��: std::ios::in)
     *
     * @details
     * ������ ����� ������ ������ ���� ���ϴ�.
     * ���� ���⿡ �����ص� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
     */
    SafeFileHandle(const std::wstring& path, std::ios::openmode mode = std::ios::in);
    
    /**
     * @brief SafeFileHandle �Ҹ���
     *
     * @details
     * ������ �����ִ� ��� �ڵ����� �ݽ��ϴ�.
     */
    ~SafeFileHandle();
    
    /**
     * @brief ������ �����ִ��� Ȯ��
     *
     * @return true ������ ��������, false ������ ��������
     */
    bool is_open() const;
    
    /**
     * @brief ���� ���� ��Ʈ�� ���� ��ȯ
     *
     * @return std::ifstream ����
     *
     * @details
     * ���� std::ifstream ��ü�� ���� ������ ��ȯ�մϴ�.
     * ���� �б� �۾��� ����� �� �ֽ��ϴ�.
     */
    std::ifstream& get();
    
private:
    std::wstring m_path;          ///< ���� ���
    std::ios::openmode m_mode;    ///< ���� ���� ���
    std::ifstream m_file;         ///< ���� ���� ��Ʈ�� ��ü
};

/**
 * @brief MAPI Property Stream �Ľ��� ����ϴ� Ŭ����
 *
 * @details
 * MSG ������ MAPI Property Stream�� �Ľ��Ͽ� ���� �Ӽ����� �����ϰ� �ؼ��ϴ� Ŭ�����Դϴ�.
 * 16����Ʈ ������ Property Definition�� �м��ϰ�, Value Section���� ���� �����͸� �����մϴ�.
 * �پ��� ������ Ÿ��(String8, Unicode, Long, Binary ��)�� �����մϴ�.
 *
 * @note
 * - ����� �α� ����� �����մϴ�.
 * - �޸� ������ �ڵ����� ó���մϴ�.
 * - �پ��� MAPI Property Tag�� Type�� �����մϴ�.
 *
 * @see https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
 */
class MAPIPropertyParser {
public:
    /**
     * @brief MAPIPropertyParser ������
     *
     * @details
     * MAPIPropertyParser ��ü�� �ʱ�ȭ�մϴ�.
     * ����� �α��� Ȱ��ȭ�ϰ� �α� ������ 3(DEBUG)���� �����մϴ�.
     */
    MAPIPropertyParser();
    
    /**
     * @brief MAPIPropertyParser �Ҹ���
     *
     * @details
     * ��� �������� �Ҵ�� Property Value �����͸� �����մϴ�.
     * ���� ������ ����� �α׷� ����մϴ�.
     */
    ~MAPIPropertyParser();
    
    /**
     * @brief MAPI Property Stream �����͸� �Ľ�
     *
     * @param[in] streamData �Ľ��� ��Ʈ�� ������
     * @return true �Ľ� ����, false �Ľ� ����
     *
     * @details
     * MAPI Property Stream�� ���̳ʸ� �����͸� �м��Ͽ� ���� �Ӽ����� �����մϴ�.
     * 16����Ʈ ������ Property Definition�� �а�, Value Section���� ���� �����͸� �����մϴ�.
     *
     * @note
     * - �ּ� 16����Ʈ�� �����Ͱ� �ʿ��մϴ�.
     * - Property Definition Section�� Value Section�� �����Ͽ� ó���մϴ�.
     * - �� �Ӽ��� Ÿ�Կ� ���� ������ �ؼ��� �����մϴ�.
     */
    bool ParsePropertiesStream(const std::vector<BYTE>& streamData);
    
    /**
     * @brief ������ Property Tag�� ���ڿ� �� ��ȯ
     *
     * @param[in] propertyTag ã�� Property Tag
     * @return ���ڿ� �� (ã�� ���� ��� �� ���ڿ�)
     *
     * @details
     * ������ Property Tag�� �ش��ϴ� �Ӽ��� ã�Ƽ� ���ڿ��� �ؼ��Ͽ� ��ȯ�մϴ�.
     * ������ Ÿ�Կ� ���� ������ ��ȯ�� �����մϴ�.
     */
    std::wstring GetStringValue(WORD propertyTag);
    
    /**
     * @brief ������ Property Tag�� 32��Ʈ ���� �� ��ȯ
     *
     * @param[in] propertyTag ã�� Property Tag
     * @return 32��Ʈ ���� �� (ã�� ���� ��� 0)
     *
     * @details
     * ������ Property Tag�� �ش��ϴ� �Ӽ��� ã�Ƽ� 32��Ʈ ������ �ؼ��Ͽ� ��ȯ�մϴ�.
     * �ּ� 4����Ʈ�� �����Ͱ� �־�� �մϴ�.
     */
    DWORD GetLongValue(WORD propertyTag);
    
    /**
     * @brief ������ Property Tag�� ���̳ʸ� ������ ��ȯ
     *
     * @param[in] propertyTag ã�� Property Tag
     * @return ���̳ʸ� ������ ���� (ã�� ���� ��� �� ����)
     *
     * @details
     * ������ Property Tag�� �ش��ϴ� �Ӽ��� ã�Ƽ� ���̳ʸ� �����ͷ� ��ȯ�մϴ�.
     * std::vector<BYTE> ���·� ��ȯ�˴ϴ�.
     */
    std::vector<BYTE> GetBinaryValue(WORD propertyTag);
    
    /**
     * @brief �Ľ̵� ��� �Ӽ��� �ֿܼ� ���
     *
     * @details
     * �Ľ̵� ��� MAPI Property�� �ֿܼ� ����մϴ�.
     * �� �Ӽ��� Tag, Type, Size, Value ������ ǥ���մϴ�.
     */
    void PrintParsedProperties();
    
    /**
     * @brief hex ���Ͽ��� �����͸� �о �Ľ�
     *
     * @param[in] hexFilePath ���� hex ���� ���
     * @return true �Ľ� ����, false �Ľ� ����
     *
     * @details
     * hex ������ �� ���� �о ���̳ʸ� �����ͷ� ��ȯ�� �� �Ľ��� �����մϴ�.
     * 2�ڸ� hex ���� ����Ʈ�� ��ȯ�Ͽ� ó���մϴ�.
     *
     * @note
     * - ���� ���ڴ� �ڵ����� ���ŵ˴ϴ�.
     * - �߸��� hex ���� ��� �α׷� ����ϰ� �ǳʶݴϴ�.
     */
    bool ParseFromHexFile(const std::wstring& hexFilePath);
    
    /**
     * @brief ����� �α� Ȱ��ȭ/��Ȱ��ȭ
     *
     * @param[in] enable true: Ȱ��ȭ, false: ��Ȱ��ȭ
     */
    void EnableDebugLog(bool enable);
    
    /**
     * @brief �α� ���� ����
     *
     * @param[in] level �α� ���� (0=Error, 1=Warning, 2=Info, 3=Debug)
     */
    void SetLogLevel(int level);
    
private:
    /**
     * @brief Property Value�� ���ڿ��� �ؼ�
     *
     * @param[in] value �ؼ��� Property Value
     * @return �ؼ��� ���ڿ�
     *
     * @details
     * Property Type�� ���� ������ �ؼ� ����� �����Ͽ� ���ڿ��� ��ȯ�մϴ�.
     * �����ϴ� Ÿ��: PT_STRING8, PT_UNICODE, PT_LONG, PT_BINARY, PT_BOOLEAN
     */
    std::wstring InterpretValue(const PROPERTY_VALUE& value);
    
    /**
     * @brief 8��Ʈ ���ڿ� �����͸� �����ڵ�� ��ȯ
     *
     * @param[in] data 8��Ʈ ���ڿ� ������
     * @param[in] size ������ ũ��
     * @return ��ȯ�� �����ڵ� ���ڿ�
     *
     * @details
     * 8��Ʈ ���ڿ�(ANSI)�� �����ڵ� ���ڿ��� ��ȯ�մϴ�.
     * Windows API�� MultiByteToWideChar�� ����մϴ�.
     */
    std::wstring InterpretString8(const BYTE* data, DWORD size);
    
    /**
     * @brief �����ڵ� ���ڿ� �����͸� �ؼ�
     *
     * @param[in] data �����ڵ� ���ڿ� ������
     * @param[in] size ������ ũ��
     * @return �ؼ��� �����ڵ� ���ڿ�
     *
     * @details
     * 2����Ʈ�� ������ �����ڵ� ���ڿ��� �ؼ��մϴ�.
     * size/2 ���� ���ڷ� ��ȯ�մϴ�.
     */
    std::wstring InterpretUnicode(const BYTE* data, DWORD size);
    
    /**
     * @brief ���̳ʸ� �����͸� hex ���ڿ��� ��ȯ
     *
     * @param[in] data ���̳ʸ� ������
     * @param[in] size ������ ũ��
     * @return hex ���ڿ��� ��ȯ�� ������
     *
     * @details
     * ���̳ʸ� �����͸� hex ���ڿ��� ��ȯ�մϴ�.
     * �ִ� 16����Ʈ������ ǥ���ϰ�, �������� "..."���� ǥ���մϴ�.
     */
    std::wstring InterpretBinary(const BYTE* data, DWORD size);
    
    /**
     * @brief 32��Ʈ ���� �����͸� �ؼ�
     *
     * @param[in] data 32��Ʈ ���� ������
     * @param[in] size ������ ũ��
     * @return �ؼ��� ���� ���ڿ�
     *
     * @details
     * �ּ� 4����Ʈ�� �����͸� 32��Ʈ ������ �ؼ��Ͽ� ���ڿ��� ��ȯ�մϴ�.
     */
    std::wstring InterpretLong(const BYTE* data, DWORD size);
    
    /**
     * @brief Property Tag�� ���ڿ��� ��ȯ
     *
     * @param[in] propertyTag ��ȯ�� Property Tag
     * @return Property Tag ���ڿ�
     *
     * @details
     * �˷��� Property Tag���� ���ڿ��� ��ȯ�մϴ�.
     * �� �� ���� Tag�� "UNKNOWN_TAG"�� ��ȯ�մϴ�.
     */
    std::wstring PropertyTagToString(WORD propertyTag);
    
    /**
     * @brief Property Type�� ���ڿ��� ��ȯ
     *
     * @param[in] propertyType ��ȯ�� Property Type
     * @return Property Type ���ڿ�
     *
     * @details
     * �˷��� Property Type���� ���ڿ��� ��ȯ�մϴ�.
     * �� �� ���� Type�� "UNKNOWN_TYPE"���� ��ȯ�մϴ�.
     */
    std::wstring PropertyTypeToString(WORD propertyType);
    
    /**
     * @brief ����� ���� �α� ���
     *
     * @param[in] message ����� �޽���
     *
     * @details
     * ����� �α��� Ȱ��ȭ�Ǿ� �ְ� �α� ������ 3 �̻��� ���� ����մϴ�.
     */
    void LogDebug(const std::wstring& message);
    
    /**
     * @brief ���� ���� �α� ���
     *
     * @param[in] message ����� �޽���
     *
     * @details
     * ����� �α��� Ȱ��ȭ�Ǿ� �ְ� �α� ������ 2 �̻��� ���� ����մϴ�.
     */
    void LogInfo(const std::wstring& message);
    
    /**
     * @brief ��� ���� �α� ���
     *
     * @param[in] message ����� �޽���
     *
     * @details
     * ����� �α��� Ȱ��ȭ�Ǿ� �ְ� �α� ������ 1 �̻��� ���� ����մϴ�.
     */
    void LogWarning(const std::wstring& message);
    
    /**
     * @brief ���� ���� �α� ���
     *
     * @param[in] message ����� �޽���
     *
     * @details
     * ����� �α��� Ȱ��ȭ�Ǿ� �ְ� �α� ������ 0 �̻��� ���� ����մϴ�.
     */
    void LogError(const std::wstring& message);
    
    /**
     * @brief ���̳ʸ� �����͸� hex dump ���·� ���
     *
     * @param[in] prefix ����� ���λ�
     * @param[in] data ����� ���̳ʸ� ������
     * @param[in] size ������ ũ��
     * @param[in] maxBytes �ִ� ����� ����Ʈ �� (�⺻��: 32)
     *
     * @details
     * ���̳ʸ� �����͸� hex ���·� ����մϴ�.
     * ����� �α��� Ȱ��ȭ�Ǿ� �ְ� �α� ������ 3 �̻��� ���� ����մϴ�.
     */
    void LogHexDump(const std::wstring& prefix, const BYTE* data, DWORD size, DWORD maxBytes = 32);
    
private:
    std::vector<PROPERTY_VALUE> m_properties;  ///< �Ľ̵� �Ӽ����� ����
    bool m_debugEnabled;                       ///< ����� �α� Ȱ��ȭ ����
    int m_logLevel;                            ///< �α� ���� (0=Error, 1=Warning, 2=Info, 3=Debug)
};