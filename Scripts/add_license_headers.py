#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
GNU GPL 3.0 라이선스 헤더 추가 스크립트
MSG File Parser Tool 프로젝트용

사용법: python add_license_headers.py
"""

import os
import glob
import re

# 라이선스 헤더 템플릿
LICENSE_HEADER = '''/*
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

'''

def should_skip_file(filename):
    """제외할 파일인지 확인"""
    skip_patterns = [
        'pch',  # 미리 컴파일된 헤더
        'resource.h',  # 자동 생성 파일
        'IMessageParser.rc',  # 리소스 파일
        'bitmap1.bmp',  # 비트맵 파일
    ]
    
    for pattern in skip_patterns:
        if pattern in filename:
            return True
    return False

def has_license_header(content):
    """파일에 이미 라이선스 헤더가 있는지 확인"""
    return "MSG File Parser Tool - Advanced MSG file analysis tool" in content

def add_license_header(filepath):
    """파일에 라이선스 헤더 추가"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # 이미 라이선스 헤더가 있으면 건너뛰기
        if has_license_header(content):
            print(f"[INFO] 이미 라이선스 헤더가 있음: {filepath}")
            return False
        
        # 파일 시작 부분에 라이선스 헤더 추가
        new_content = LICENSE_HEADER + content
        
        # 백업 파일 생성
        backup_path = filepath + '.backup'
        with open(backup_path, 'w', encoding='utf-8') as f:
            f.write(content)
        
        # 새 내용으로 파일 덮어쓰기
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        
        print(f"[INFO] 라이선스 헤더 추가됨: {filepath}")
        return True
        
    except Exception as e:
        print(f"[ERROR] 오류 발생 ({filepath}): {e}")
        return False

def main():
    """메인 함수"""
    print("[INFO] GNU GPL 3.0 라이선스 헤더 추가 스크립트")
    print("=" * 50)
    
    # Src 디렉토리에서 .cpp와 .h 파일 찾기
    src_dir = "Src"
    if not os.path.exists(src_dir):
        print(f"[ERROR] {src_dir} 디렉토리를 찾을 수 없습니다.")
        return
    
    # 파일 패턴
    patterns = [
        os.path.join(src_dir, "*.cpp"),
        os.path.join(src_dir, "*.h")
    ]
    
    target_files = []
    for pattern in patterns:
        target_files.extend(glob.glob(pattern))
    
    # 제외할 파일 필터링
    target_files = [f for f in target_files if not should_skip_file(f)]
    
    if not target_files:
        print("[INFO] 처리할 파일을 찾을 수 없습니다.")
        return
    
    print(f"[INFO] 처리할 파일 목록 ({len(target_files)}개):")
    for file in target_files:
        print(f"   - {os.path.basename(file)}")
    print()
    
    # 사용자 확인
    response = input("계속하시겠습니까? (y/N): ").strip().lower()
    if response not in ['y', 'yes']:
        print("[INFO] 취소되었습니다.")
        return
    
    print("\n[INFO] 라이선스 헤더 추가 중...")
    print("-" * 30)
    
    success_count = 0
    skip_count = 0
    error_count = 0
    
    for filepath in target_files:
        if add_license_header(filepath):
            success_count += 1
        else:
            skip_count += 1
    
    print("-" * 30)
    print("[INFO] 처리 결과:")
    print(f"   [INFO] 성공: {success_count}개")
    print(f"   [INFO] 건너뜀: {skip_count}개")
    print(f"   [ERROR] 오류: {error_count}개")
    print()
    
    if success_count > 0:
        print("[INFO] 백업 파일이 .backup 확장자로 생성되었습니다.")
        print("   문제가 없으면 백업 파일을 삭제하세요.")
    
    print("[INFO] 완료!")

if __name__ == "__main__":
    main() 