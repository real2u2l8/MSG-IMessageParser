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

#include "pch.h"
#include "analysis.h"
#include "utils.h"
#include "ui.h"
#include "privilege.h"
#include "version.h"

int main(int argc, char* argv[]) {
    // 콘솔 출력을 위한 로케일 설정
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
    
    Version::ShowBanner();
    std::wcout << std::endl;
    
    // 관리자 권한 확인
    if (!Privilege::IsRunningAsAdministrator()) {
        std::wcerr << L"[ERROR] Administrator privileges are required to access MSG files." << std::endl;
        std::wcerr << L"[ERROR] Please run this program as Administrator." << std::endl;
        std::wcout << std::endl;
        UI::PrintUsage();
        return 1;
    }
    
    // 명령행 인자 검사
    if (argc < 2) {
        std::wcerr << L"[ERROR] MSG file path is required." << std::endl;
        std::wcout << std::endl;
        UI::PrintUsage();
        return 1;
    }
    
    // MSG 파일 경로 (char*를 wstring으로 변환)
    std::wstring msgFilePath;
    int len = MultiByteToWideChar(CP_ACP, 0, argv[1], -1, nullptr, 0);
    if (len > 0) {
        wchar_t* wstr = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, argv[1], -1, wstr, len);
        msgFilePath = wstr;
        delete[] wstr;
    }
    
    // 출력 디렉토리 (선택적)
    std::wstring outputDir;
    if (argc >= 3) {
        int len2 = MultiByteToWideChar(CP_ACP, 0, argv[2], -1, nullptr, 0);
        if (len2 > 0) {
            wchar_t* wstr2 = new wchar_t[len2];
            MultiByteToWideChar(CP_ACP, 0, argv[2], -1, wstr2, len2);
            outputDir = wstr2;
            delete[] wstr2;
        }
    }
    
    // 파일 경로 유효성 검사 (Utils 네임스페이스 사용)
    if (!Utils::ValidateFilePath(msgFilePath)) {
        return 1;
    }
    
    // 절대 경로로 변환 (Utils 네임스페이스 사용)
    msgFilePath = Utils::GetAbsolutePath(msgFilePath);
    std::wcout << L"[INFO] MSG file path: " << msgFilePath << std::endl;
    
    // 출력 디렉토리 처리 (Utils 네임스페이스 사용)
    if (outputDir.empty()) {
        // 자동 디렉토리 생성
        outputDir = Utils::GenerateOutputDirectory(msgFilePath);
        if (outputDir.empty()) {
            std::wcerr << L"[ERROR] Failed to generate output directory." << std::endl;
            return 1;
        }
        std::wcout << L"[INFO] Auto-generated output directory: " << outputDir << std::endl;
    } else {
        // 사용자 지정 디렉토리 검증 및 생성
        if (!Utils::ValidateAndCreateOutputDir(outputDir)) {
            return 1;
        }
        outputDir = Utils::GetAbsolutePath(outputDir);
        std::wcout << L"[INFO] User-specified output directory: " << outputDir << std::endl;
    }
    
    std::wcout << std::endl;
    
    try {
        // MSG 파서 생성 및 실행
        MSGParser parser;
        
        std::wcout << L"[INFO] Starting MSG file analysis..." << std::endl;
        std::wcout << L"[INFO] This may take a few moments depending on file size." << std::endl;
        std::wcout << std::endl;
        
        bool success = parser.ParseMSGFile(msgFilePath, outputDir);
        
        std::wcout << std::endl;
        if (success) {
            std::wcout << L"[SUCCESS] MSG file analysis completed successfully." << std::endl;
            std::wcout << L"[INFO] Analysis results saved to: " << outputDir << std::endl;
            std::wcout << L"[INFO] Check the output directory for detailed file structure and data." << std::endl;
            
            // 명시적 리소스 정리
            std::wcout << L"[INFO] Cleaning up resources..." << std::endl;
            
            return 0;
        } else {
            std::wcerr << L"[ERROR] Error occurred during MSG file analysis." << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::wcerr << L"[ERROR] Exception occurred: ";
        std::wcerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::wcerr << L"[ERROR] Unknown exception occurred." << std::endl;
        return 1;
    }
}

