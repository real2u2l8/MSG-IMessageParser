/*
 * MSG File Parser Tool - Advanced MSG file analysis tool with MAPI property parsing
 * Copyright (C) 2025  real2u2l8
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "pch.h"

/**
 * 권한 관리 관련 함수들을 모아놓은 네임스페이스
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
