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

// 250617.Outmail.exe ���� �м� �߰�. by. real.hansy
// �̸��� �ּ� Ÿ���� �����ϴ� ������
// SMTP: �Ϲ����� ���ͳ� �̸��� �ּ� ���� (��: user@domain.com)
// EX: Exchange �������� ����ϴ� ���� �ּ� ����
// UNKNOWN: �� �� ���� �ּ� ����
enum class AddrType
{
	SMTP,
	EX,
	UNKNOWN
};

// 250618.Outmail.exe ���� �м� �߰�. by. real.hansy
/**
 * @brief ������ �ּ� ĳ�� ����ü
 *
 * @details
 * - ������ ����(TO, CC, BCC)�� �ӽ÷� �����ϱ� ���� ����ü�Դϴ�.
 * - �Ľ� �������� ����� ���ڿ��� Ÿ�Ժ��� �з��Ͽ� �����ϰ�,
 * - ���������� ���� �Լ����� mail_info ����ü�� ��ȯ�Ͽ� ����մϴ�.
 *
 * @note
 * - ������ ������ ��Ȯ�� �ϱ� ���� TO/CC/BCC�� ���������� �и�������,
 * - Ÿ�Ժ��� �����Ͽ� ���������ν� �ļ� ó������ �α� �� �߰� Ȱ���� �����մϴ�.
 * - STL map ��� C ��Ÿ�� ����ü�� ����Ͽ� ó�� �ӵ��� ����ȭ�߽��ϴ�.
 */
typedef struct _RecipCache {
	char* pszTo;		// To �ּ� ���ڿ� (�޸�/�����Ҵ����� �����ؾ� ��)
	char* pszCc;		// Cc �ּ� ���ڿ� (�޸�/�����Ҵ����� �����ؾ� ��)
	char* pszBcc;	    // Bcc �ּ� ���ڿ� (�޸�/�����Ҵ����� �����ؾ� ��)

	int   nToType;   // 1 = TO
	int   nCcType;   // 2 = CC
	int   nBccType;  // 3 = BCC
} RecipCache;

// MSG ���� �Ľ� ���� �Լ� �����
class MSGParser {
public:
    MSGParser();
    ~MSGParser();
    
    // ���� �Ľ� �Լ�
    bool ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir);
    
private:
    // ����� ���丮��/��Ʈ�� Ž��
    void TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir);
    
    // ��Ʈ�� �����͸� hex�� ��ȯ�Ͽ� ���Ϸ� ����
    bool SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath);
    
    // ���̳ʸ� �����͸� hex ���ڿ��� ��ȯ
    std::string BinaryToHexString(const BYTE* data, DWORD size);
    
    // ������ ���ϸ� ���� (Ư������ ó��)
    std::wstring MakeSafeFileName(const std::wstring& originalName);
    
    // ���丮 ����
    bool CreateDirectoryRecursive(const std::wstring& path);
    
    // ��ƿ��Ƽ �Լ���
    void LogMessage(const std::wstring& message);
    void LogError(const std::wstring& error);
    
private:
    std::wstring m_outputBaseDir;
    int m_fileCount;
    int m_errorCount;
};