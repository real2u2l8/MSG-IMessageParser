#pragma once
#include "pch.h"

/**
 * @brief 유틸리티 함수들을 모아놓은 네임스페이스
 * 
 * @details
 * 파일 경로 처리, 디렉토리 관리, 날짜/시간 처리 등
 * 공통적으로 사용되는 유틸리티 함수들을 제공합니다.
 */
namespace Utils {

    /**
     * @brief 현재 날짜와 시간을 문자열로 반환
     * 
     * @return YYYYMMDD_HHMMSS 형식의 문자열
     * 
     * @details
     * 시스템의 현재 시간을 가져와서 파일명에 사용할 수 있는
     * 형식으로 변환합니다.
     * 
     * @example
     * std::wstring timestamp = Utils::GetCurrentDateTime();
     * // 결과: "20241201_143052"
     */
    std::wstring GetCurrentDateTime();

    /**
     * @brief 파일명에서 확장자 제거
     * 
     * @param[in] fullPath 전체 파일 경로
     * @return 확장자가 제거된 파일명
     * 
     * @details
     * 파일 경로에서 파일명만 추출하고 확장자를 제거합니다.
     * 
     * @example
     * std::wstring name = Utils::GetFileNameWithoutExtension(L"C:\\test\\file.msg");
     * // 결과: "file"
     */
    std::wstring GetFileNameWithoutExtension(const std::wstring& fullPath);

    /**
     * @brief 전체 경로에서 파일명만 추출
     * 
     * @param[in] fullPath 전체 파일 경로
     * @return 파일명 (확장자 포함)
     * 
     * @details
     * 경로 구분자(\\ 또는 /)를 기준으로 파일명만 추출합니다.
     * 
     * @example
     * std::wstring name = Utils::GetFileName(L"C:\\test\\file.msg");
     * // 결과: "file.msg"
     */
    std::wstring GetFileName(const std::wstring& fullPath);

    /**
     * @brief 상대 경로를 절대 경로로 변환
     * 
     * @param[in] relativePath 상대 경로
     * @return 절대 경로
     * 
     * @details
     * Windows API의 GetFullPathNameW를 사용하여
     * 상대 경로를 절대 경로로 변환합니다.
     * 변환에 실패하면 원본 경로를 반환합니다.
     * 
     * @example
     * std::wstring absPath = Utils::GetAbsolutePath(L".\\test.msg");
     * // 결과: "C:\\current\\directory\\test.msg"
     */
    std::wstring GetAbsolutePath(const std::wstring& relativePath);

    /**
     * @brief MSG 파일 경로 유효성 검사
     * 
     * @param[in] filePath 검사할 파일 경로
     * @return true 유효한 MSG 파일, false 유효하지 않음
     * 
     * @details
     * 다음 조건들을 검사합니다:
     * - 파일이 실제로 존재하는지
     * - 디렉토리가 아닌 파일인지
     * - .msg 확장자를 가지고 있는지
     * 
     * @note
     * 검사 실패 시 구체적인 에러 메시지를 표준 에러 출력에 출력합니다.
     */
    bool ValidateFilePath(const std::wstring& filePath);

    /**
     * @brief 자동 출력 디렉토리 생성
     * 
     * @param[in] msgFilePath MSG 파일 경로
     * @return 생성된 출력 디렉토리 경로 (실패 시 빈 문자열)
     * 
     * @details
     * MSG 파일명과 현재 시간을 조합하여 자동으로 출력 디렉토리를 생성합니다.
     * 형식: YYYYMMDD_HHMMSS_filename_data
     * 
     * @example
     * std::wstring outputDir = Utils::GenerateOutputDirectory(L"C:\\test.msg");
     * // 결과: "C:\\current\\20241201_143052_test_data"
     */
    std::wstring GenerateOutputDirectory(const std::wstring& msgFilePath);

    /**
     * @brief 출력 디렉토리 생성 및 검증
     * 
     * @param[in,out] outputDir 출력 디렉토리 경로 (빈 문자열이면 자동 생성)
     * @return true 성공, false 실패
     * 
     * @details
     * 출력 디렉토리가 비어있으면 자동으로 생성하고,
     * 지정된 경로가 있으면 해당 디렉토리를 검증하고 생성합니다.
     * 
     * @note
     * outputDir이 빈 문자열이면 GenerateOutputDirectory를 호출하여
     * 자동으로 디렉토리를 생성합니다.
     */
    bool ValidateAndCreateOutputDir(std::wstring& outputDir);

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
     * Windows API의 CreateDirectoryW를 사용합니다.
     * ERROR_ALREADY_EXISTS 에러는 성공으로 처리합니다.
     */
    bool CreateDirectoryRecursive(const std::wstring& path);

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
     * 
     * @example
     * std::wstring safeName = Utils::MakeSafeFileName(L"file<name>:test");
     * // 결과: "file_name_test"
     */
    std::wstring MakeSafeFileName(const std::wstring& originalName);

} // namespace Utils

