#pragma once
#include "pch.h"

/**
 * @brief 사용자 인터페이스 관련 함수들을 모아놓은 네임스페이스
 * 
 * @details
 * 프로그램 실행 시 사용자에게 보여주는 화면과 메시지들을 처리합니다.
 * 사용법 출력, 배너 표시, 에러 메시지 등이 포함됩니다.
 */
namespace UI {

    /**
     * @brief 프로그램 사용법을 출력
     * 
     * @param[in] programName 프로그램 이름
     * 
     * @details
     * 프로그램의 사용법, 인자 설명, 예제, 기능 설명 등을
     * 콘솔에 출력합니다.
     * 
     * @example
     * UI::PrintUsage();
     */
    void PrintUsage();

} // namespace UI
