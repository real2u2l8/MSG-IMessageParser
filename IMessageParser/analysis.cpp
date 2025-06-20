#include "pch.h"
#include "analysis.h"

// OutLook Mail�� .msg ������ ���� ����, ���⼭ ������ Substg�� Ȯ���� �� �ִ�. by. real.hansy
// https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a
// http://www.five-ten-sg.com/libpst/rn01re06.html
// OutLook Mail�� .msg ���� ������ ������ ���� �ٲ� �� �ִ�.
// KESM_OutMail.exe�� ���� ��������� .msg ���������� ��Ȥ �̽��� �߻��Ѵ�.
// ���� ���丮�� ���� �ʵ� 
#define		SUBSTG_SENDER		L"__substg1.0_0C1F"		// PR_SENDER_EMAIL_ADDRESS_W:	�۽����� �̸��� �ּ� (Exchange ���� ���� ����)
#define		SUBSTG_TO			L"__substg1.0_0E04"		// PR_DISPLAY_TO_W:				������(To) ��ü ǥ�� ���ڿ�,	�ּҰ� �ƴ� �� ����
#define		SUBSTG_CC			L"__substg1.0_0E03"		// PR_DISPLAY_CC_W:				����(CC) ��ü ǥ�� ���ڿ�		�ּҰ� �ƴ� �� ����
#define		SUBSTG_BCC			L"__substg1.0_0E02"		// PR_DISPLAY_BCC_W:			��������(BCC) ��ü ǥ�� ���ڿ�	�ּҰ� �ƴ� �� ����
#define		SUBSTG_SUBJECT		L"__substg1.0_0037"		// PR_SUBJECT_W:				���� ����
#define		SUBSTG_BODY			L"__substg1.0_1000"		// PR_BODY_W:					���� ���� (�ؽ�Ʈ)
#define		SUBSTG_HEADER		L"__substg1.0_007D"		// PR_SUBJECT_PREFIX:			���� �� ���ξ� ("RE:", "FW:" ��)
// ���� ���丮��
#define     MAIL_MAIN			L"__nameid_version"		// Named Property�� �̸�-GUID-Tag ���� ������ ��� ��Ÿ ���丮��
#define     MAIL_SUBTO			L"__recip_version"		// �� ������(TO/CC/BCC)�� Recipient Table ���丮��
#define		MAIL_PROPERTY		L"__properties_version"	// ���� ���丮�� ���� ���� �Ӽ��� ��� ���丮��
#define     MAIL_ATTACH			L"__attach_version"		// ÷������ ���� �Ӽ��� �����͸� ��� Attach ���丮��
// __attach_version ���� Ʈ��
#define		SUBSTG_ATTACHDISPLAY	L"__substg1.0_3001"		// PR_DISPLAY_NAME_W:        ÷������ ǥ�� �̸�
#define		SUBSTG_ATTACHNAME		L"__substg1.0_3707"		// PR_ATTACH_LONG_FILENAME_W: ÷������ ���� �̸� (�� ����)
#define		SUBSTG_ATTACHDATA		L"__substg1.0_3701"		// PR_ATTACH_DATA_BIN:        ÷������ ���̳ʸ� ������
// Ư�� ��ũ��
#define		SUBSTG_ADDRTYPE		    L"__substg1.0_3002"		// PR_ADDRTYPE_W:             �ּ� Ÿ�� (SMTP, EX ��)
#define		SUBSTG_SENDER_ADDRTYPE	L"__substg1.0_0C1E"		// PR_SENDER_ADDRTYPE_W:      �۽��� �ּ� Ÿ�� (SMTP, EX ��)
#define		SUBSTG_SMTPADDRESS		L"__substg1.0_39FE"		// PR_SMTP_ADDRESS_W:         EX Ÿ���� ���� �̸��� �ּ�
#define		SUBSTG_SUBTO		    L"__substg1.0_3003"		// PR_EMAIL_ADDRESS_W:        ������ �̸��� �ּ�
#define		SUBSTG_RECIPIENT_TYPE	L"__substg1.0_0C15"		// PR_RECIPIENT_TYPE:		������ ���� ���� (To, Cc, Bcc)
// _recip_version ���� Ʈ��
#define		RECIP_TYPE_TO			1       // ������(To)
#define		RECIP_TYPE_CC			2       // ����(Cc)
#define		RECIP_TYPE_BCC			3       // ��������(Bcc)


#define		MAX_RECIPIENT_ADDR_LEN	256  // TO/CC/BCC ���տ�

// 250617.Outmail.exe ���ϴ��� �м� �̽�. by. real.hansy
// �̸��� �ּ� Ÿ���� ��Ÿ���� ������
// SMTP: �Ϲ����� ���ͳ� �̸��� �ּ� ���� (��: user@domain.com)
// EX: Exchange �������� ����ϴ� ���� �ּ� ����
// UNKNOWN: �� �� ���� �ּ� ����
enum class AddrType
{
	SMTP,
	EX,
	UNKNOWN
};

// 250618.Outmail.exe ���ϴ��� �м� �̽�. by. real.hansy
/**
 * @brief ������ �ּ� ĳ�� ����ü
 *
 * @details
 * - ������ ������(TO, CC, BCC) ������ �ӽ÷� �����ϱ� ���� ����ü�Դϴ�.
 * - �Ľ� ���� ����� ���� ���ڿ��� Ÿ�Ժ��� �����Ͽ� �����ϸ�,
 * - ��ó�� �ܰ迡�� mail_info ����ü�� ���޵Ǳ� �� ToContentsParse() �Լ��� �����˴ϴ�.
 *
 * @note
 * - ������ ������ ��Ȯ�� �ϱ� ���� TO/CC/BCC�� ���� ����� �и��Ͽ�����,
 * - Ÿ�԰��� �Բ� ���������ν� ���� ���� �б� ó���� �α� �� �߰� Ȱ���� �����մϴ�.
 * - STL map ���� ���� �����̳� ��� ����ü �������� ����Ͽ� ó�� �ӵ��� �ڵ� ��Ἲ�� Ȯ���Ͽ����ϴ�.
 */
typedef struct _RecipCache {
	char* pszTo;		// To �ּ� ���ڿ� (�޸�/�����ݷ� ���Ե� ���� �״��)
	char* pszCc;		// Cc �ּ� ���ڿ� (�޸�/�����ݷ� ���Ե� ���� �״��)
	char* pszBcc;	    // Bcc �ּ� ���ڿ� (�޸�/�����ݷ� ���Ե� ���� �״��)

	int   nToType;   // 1 = TO
	int   nCcType;   // 2 = CC
	int   nBccType;  // 3 = BCC
} RecipCache;

// ���⿡ ���� �Լ� �������� �� �����Դϴ�.
// ����� ��� ���ǰ� analysis.h�� �̵��Ǿ����ϴ�.

// MSGParser Ŭ���� ����
MSGParser::MSGParser() : m_fileCount(0), m_errorCount(0) {
    // COM �ʱ�ȭ
    CoInitialize(nullptr);
}

MSGParser::~MSGParser() {
    // COM ����
    CoUninitialize();
}

bool MSGParser::ParseMSGFile(const std::wstring& filePath, const std::wstring& outputDir) {
    LogMessage(L"MSG ���� �Ľ� ����: " + filePath);
    
    m_outputBaseDir = outputDir;
    m_fileCount = 0;
    m_errorCount = 0;
    
    // ��� ���丮 ����
    if (!CreateDirectoryRecursive(outputDir)) {
        LogError(L"��� ���丮 ���� ����: " + outputDir);
        return false;
    }
    
    // IStorage�� MSG ���� ����
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
        LogError(L"MSG ���� ���� ����: " + filePath);
        return false;
    }
    
    // ��Ʈ ���丮������ ����� Ž�� ����
    TraverseStorage(pStorage, L"", outputDir);
    
    // ���丮�� ����
    pStorage->Release();
    
    LogMessage(L"�Ľ� �Ϸ� - ó���� ����: " + std::to_wstring(m_fileCount) + 
               L", ����: " + std::to_wstring(m_errorCount));
    
    return m_errorCount == 0;
}

void MSGParser::TraverseStorage(IStorage* pStorage, const std::wstring& currentPath, const std::wstring& outputDir) {
    if (!pStorage) return;
    
    // ���� ���丮���� ��� ��� ����
    IEnumSTATSTG* pEnum = nullptr;
    HRESULT hr = pStorage->EnumElements(0, nullptr, 0, &pEnum);
    
    if (FAILED(hr) || !pEnum) {
        LogError(L"���丮�� ���� ����: " + currentPath);
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
            // ���丮���� ��� - ���� ���� �� ��� Ž��
            LogMessage(L"���丮�� �߰�: " + fullPath);
            
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
                    LogError(L"���� ���丮�� ���� ����: " + fullPath);
                    m_errorCount++;
                }
            } else {
                LogError(L"���丮 ���� ����: " + elementOutputPath);
                m_errorCount++;
            }
        }
        else if (stat.type == STGTY_STREAM) {
            // ��Ʈ���� ��� - hex ���Ϸ� ����
            LogMessage(L"��Ʈ�� �߰�: " + fullPath);
            
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
                LogError(L"��Ʈ�� ���� ����: " + fullPath);
                m_errorCount++;
            }
        }
        
        // �޸� ����
        if (stat.pwcsName) {
            CoTaskMemFree(stat.pwcsName);
        }
    }
    
    pEnum->Release();
}

bool MSGParser::SaveStreamAsHex(IStream* pStream, const std::wstring& outputPath) {
    if (!pStream) return false;
    
    // ��Ʈ�� ũ�� Ȯ��
    STATSTG stat;
    HRESULT hr = pStream->Stat(&stat, STATFLAG_NONAME);
    if (FAILED(hr)) {
        LogError(L"��Ʈ�� ũ�� Ȯ�� ����: " + outputPath);
        return false;
    }
    
    DWORD streamSize = stat.cbSize.LowPart;
    if (streamSize == 0) {
        LogMessage(L"�� ��Ʈ�� �ǳʶ�: " + outputPath);
        return true;
    }
    
    // ��Ʈ�� ������ �б�
    std::vector<BYTE> buffer(streamSize);
    ULONG bytesRead = 0;
    
    hr = pStream->Read(buffer.data(), streamSize, &bytesRead);
    if (FAILED(hr) || bytesRead != streamSize) {
        LogError(L"��Ʈ�� ������ �б� ����: " + outputPath);
        return false;
    }
    
    // hex ���ڿ��� ��ȯ
    std::string hexString = BinaryToHexString(buffer.data(), bytesRead);
    
    // ���Ϸ� ����
    std::ofstream file(outputPath);
    if (!file.is_open()) {
        LogError(L"hex ���� ���� ����: " + outputPath);
        return false;
    }
    
    file << hexString;
    file.close();
    
    LogMessage(L"hex ���� ���� �Ϸ�: " + outputPath + L" (" + std::to_wstring(bytesRead) + L" bytes)");
    return true;
}

std::string MSGParser::BinaryToHexString(const BYTE* data, DWORD size) {
    if (!data || size == 0) return "";
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::uppercase;
    
    for (DWORD i = 0; i < size; i++) {
        if (i > 0 && i % 16 == 0) {
            ss << std::endl;  // 16����Ʈ���� �ٹٲ�
        }
        ss << std::setw(2) << static_cast<int>(data[i]);
        
        if (i < size - 1 && (i + 1) % 16 != 0) {
            ss << " ";  // ����Ʈ ���� ����
        }
    }
    
    return ss.str();
}

std::wstring MSGParser::MakeSafeFileName(const std::wstring& originalName) {
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

bool MSGParser::CreateDirectoryRecursive(const std::wstring& path) {
    if (path.empty()) return false;
    
    // �̹� �����ϴ��� Ȯ��
    DWORD attrs = GetFileAttributes(path.c_str());
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
    return CreateDirectory(path.c_str(), nullptr) == TRUE || 
           GetLastError() == ERROR_ALREADY_EXISTS;
}

void MSGParser::LogMessage(const std::wstring& message) {
    std::wcout << L"[INFO] " << message << std::endl;
}

void MSGParser::LogError(const std::wstring& error) {
    std::wcerr << L"[ERROR] " << error << std::endl;
}