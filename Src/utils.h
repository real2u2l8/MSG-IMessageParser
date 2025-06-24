#pragma once
#include "pch.h"

/**
 * @brief ��ƿ��Ƽ �Լ����� ��Ƴ��� ���ӽ����̽�
 * 
 * @details
 * ���� ��� ó��, ���丮 ����, ��¥/�ð� ó�� ��
 * ���������� ���Ǵ� ��ƿ��Ƽ �Լ����� �����մϴ�.
 */
namespace Utils {

    /**
     * @brief ���� ��¥�� �ð��� ���ڿ��� ��ȯ
     * 
     * @return YYYYMMDD_HHMMSS ������ ���ڿ�
     * 
     * @details
     * �ý����� ���� �ð��� �����ͼ� ���ϸ� ����� �� �ִ�
     * �������� ��ȯ�մϴ�.
     * 
     * @example
     * std::wstring timestamp = Utils::GetCurrentDateTime();
     * // ���: "20241201_143052"
     */
    std::wstring GetCurrentDateTime();

    /**
     * @brief ���ϸ��� Ȯ���� ����
     * 
     * @param[in] fullPath ��ü ���� ���
     * @return Ȯ���ڰ� ���ŵ� ���ϸ�
     * 
     * @details
     * ���� ��ο��� ���ϸ� �����ϰ� Ȯ���ڸ� �����մϴ�.
     * 
     * @example
     * std::wstring name = Utils::GetFileNameWithoutExtension(L"C:\\test\\file.msg");
     * // ���: "file"
     */
    std::wstring GetFileNameWithoutExtension(const std::wstring& fullPath);

    /**
     * @brief ��ü ��ο��� ���ϸ� ����
     * 
     * @param[in] fullPath ��ü ���� ���
     * @return ���ϸ� (Ȯ���� ����)
     * 
     * @details
     * ��� ������(\\ �Ǵ� /)�� �������� ���ϸ� �����մϴ�.
     * 
     * @example
     * std::wstring name = Utils::GetFileName(L"C:\\test\\file.msg");
     * // ���: "file.msg"
     */
    std::wstring GetFileName(const std::wstring& fullPath);

    /**
     * @brief ��� ��θ� ���� ��η� ��ȯ
     * 
     * @param[in] relativePath ��� ���
     * @return ���� ���
     * 
     * @details
     * Windows API�� GetFullPathNameW�� ����Ͽ�
     * ��� ��θ� ���� ��η� ��ȯ�մϴ�.
     * ��ȯ�� �����ϸ� ���� ��θ� ��ȯ�մϴ�.
     * 
     * @example
     * std::wstring absPath = Utils::GetAbsolutePath(L".\\test.msg");
     * // ���: "C:\\current\\directory\\test.msg"
     */
    std::wstring GetAbsolutePath(const std::wstring& relativePath);

    /**
     * @brief MSG ���� ��� ��ȿ�� �˻�
     * 
     * @param[in] filePath �˻��� ���� ���
     * @return true ��ȿ�� MSG ����, false ��ȿ���� ����
     * 
     * @details
     * ���� ���ǵ��� �˻��մϴ�:
     * - ������ ������ �����ϴ���
     * - ���丮�� �ƴ� ��������
     * - .msg Ȯ���ڸ� ������ �ִ���
     * 
     * @note
     * �˻� ���� �� ��ü���� ���� �޽����� ǥ�� ���� ��¿� ����մϴ�.
     */
    bool ValidateFilePath(const std::wstring& filePath);

    /**
     * @brief �ڵ� ��� ���丮 ����
     * 
     * @param[in] msgFilePath MSG ���� ���
     * @return ������ ��� ���丮 ��� (���� �� �� ���ڿ�)
     * 
     * @details
     * MSG ���ϸ�� ���� �ð��� �����Ͽ� �ڵ����� ��� ���丮�� �����մϴ�.
     * ����: YYYYMMDD_HHMMSS_filename_data
     * 
     * @example
     * std::wstring outputDir = Utils::GenerateOutputDirectory(L"C:\\test.msg");
     * // ���: "C:\\current\\20241201_143052_test_data"
     */
    std::wstring GenerateOutputDirectory(const std::wstring& msgFilePath);

    /**
     * @brief ��� ���丮 ���� �� ����
     * 
     * @param[in,out] outputDir ��� ���丮 ��� (�� ���ڿ��̸� �ڵ� ����)
     * @return true ����, false ����
     * 
     * @details
     * ��� ���丮�� ��������� �ڵ����� �����ϰ�,
     * ������ ��ΰ� ������ �ش� ���丮�� �����ϰ� �����մϴ�.
     * 
     * @note
     * outputDir�� �� ���ڿ��̸� GenerateOutputDirectory�� ȣ���Ͽ�
     * �ڵ����� ���丮�� �����մϴ�.
     */
    bool ValidateAndCreateOutputDir(std::wstring& outputDir);

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
     * Windows API�� CreateDirectoryW�� ����մϴ�.
     * ERROR_ALREADY_EXISTS ������ �������� ó���մϴ�.
     */
    bool CreateDirectoryRecursive(const std::wstring& path);

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
     * 
     * @example
     * std::wstring safeName = Utils::MakeSafeFileName(L"file<name>:test");
     * // ���: "file_name_test"
     */
    std::wstring MakeSafeFileName(const std::wstring& originalName);

} // namespace Utils

