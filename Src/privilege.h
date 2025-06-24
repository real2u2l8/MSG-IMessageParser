#pragma once
#include "pch.h"

/**
 * @brief ���� ���� ���� �Լ����� ��Ƴ��� ���ӽ����̽�
 * 
 * @details
 * ���α׷� ���࿡ �ʿ��� ���ѵ��� Ȯ���ϰ� �����մϴ�.
 * ������ ���� Ȯ��, Ư�� ���� ���� ���Ե˴ϴ�.
 */
namespace Privilege {

    /**
     * @brief ���� ���μ����� ������ �������� ����ǰ� �ִ��� Ȯ��
     * 
     * @return true ������ �������� ���� ��, false �Ϲ� ����� ����
     * 
     * @details
     * Windows API�� ����Ͽ� ���� ���μ����� Administrators �׷쿡
     * ���ϴ��� Ȯ���մϴ�. MSG ���� ���ٿ� ������ ������ �ʿ��ϹǷ�
     * ���α׷� ���� �� �� �Լ��� ȣ���Ͽ� ������ �����մϴ�.
     * 
     * @note
     * - CheckTokenMembership API�� ����մϴ�.
     * - Administrators �׷��� SID�� �����Ͽ� Ȯ���մϴ�.
     * - ���� Ȯ�ο� �����ϸ� false�� ��ȯ�մϴ�.
     * 
     * @example
     * if (!Privilege::IsRunningAsAdministrator()) {
     *     std::wcerr << L"Administrator privileges required!" << std::endl;
     *     return 1;
     * }
     */
    bool IsRunningAsAdministrator();

} // namespace Privilege
