#pragma once
#include "pch.h"

/**
 * @brief 권한 관리 관련 함수들을 모아놓은 네임스페이스
 * 
 * @details
 * 프로그램 실행에 필요한 권한들을 확인하고 관리합니다.
 * 관리자 권한 확인, 특권 검증 등이 포함됩니다.
 */
namespace Privilege {

    /**
     * @brief 현재 프로세스가 관리자 권한으로 실행되고 있는지 확인
     * 
     * @return true 관리자 권한으로 실행 중, false 일반 사용자 권한
     * 
     * @details
     * Windows API를 사용하여 현재 프로세스가 Administrators 그룹에
     * 속하는지 확인합니다. MSG 파일 접근에 관리자 권한이 필요하므로
     * 프로그램 시작 시 이 함수를 호출하여 권한을 검증합니다.
     * 
     * @note
     * - CheckTokenMembership API를 사용합니다.
     * - Administrators 그룹의 SID를 생성하여 확인합니다.
     * - 권한 확인에 실패하면 false를 반환합니다.
     * 
     * @example
     * if (!Privilege::IsRunningAsAdministrator()) {
     *     std::wcerr << L"Administrator privileges required!" << std::endl;
     *     return 1;
     * }
     */
    bool IsRunningAsAdministrator();

} // namespace Privilege
