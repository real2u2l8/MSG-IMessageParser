#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
GNU GPL 3.0 ���̼��� ��� �߰� ��ũ��Ʈ
MSG File Parser Tool ������Ʈ��

����: python add_license_headers.py
"""

import os
import glob
import re

# ���̼��� ��� ���ø�
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
    """������ �������� Ȯ��"""
    skip_patterns = [
        'pch',  # �̸� �����ϵ� ���
        'resource.h',  # �ڵ� ���� ����
        'IMessageParser.rc',  # ���ҽ� ����
        'bitmap1.bmp',  # ��Ʈ�� ����
    ]
    
    for pattern in skip_patterns:
        if pattern in filename:
            return True
    return False

def has_license_header(content):
    """���Ͽ� �̹� ���̼��� ����� �ִ��� Ȯ��"""
    return "MSG File Parser Tool - Advanced MSG file analysis tool" in content

def add_license_header(filepath):
    """���Ͽ� ���̼��� ��� �߰�"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # �̹� ���̼��� ����� ������ �ǳʶٱ�
        if has_license_header(content):
            print(f"[INFO] �̹� ���̼��� ����� ����: {filepath}")
            return False
        
        # ���� ���� �κп� ���̼��� ��� �߰�
        new_content = LICENSE_HEADER + content
        
        # ��� ���� ����
        backup_path = filepath + '.backup'
        with open(backup_path, 'w', encoding='utf-8') as f:
            f.write(content)
        
        # �� �������� ���� �����
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        
        print(f"[INFO] ���̼��� ��� �߰���: {filepath}")
        return True
        
    except Exception as e:
        print(f"[ERROR] ���� �߻� ({filepath}): {e}")
        return False

def main():
    """���� �Լ�"""
    print("[INFO] GNU GPL 3.0 ���̼��� ��� �߰� ��ũ��Ʈ")
    print("=" * 50)
    
    # Src ���丮���� .cpp�� .h ���� ã��
    src_dir = "Src"
    if not os.path.exists(src_dir):
        print(f"[ERROR] {src_dir} ���丮�� ã�� �� �����ϴ�.")
        return
    
    # ���� ����
    patterns = [
        os.path.join(src_dir, "*.cpp"),
        os.path.join(src_dir, "*.h")
    ]
    
    target_files = []
    for pattern in patterns:
        target_files.extend(glob.glob(pattern))
    
    # ������ ���� ���͸�
    target_files = [f for f in target_files if not should_skip_file(f)]
    
    if not target_files:
        print("[INFO] ó���� ������ ã�� �� �����ϴ�.")
        return
    
    print(f"[INFO] ó���� ���� ��� ({len(target_files)}��):")
    for file in target_files:
        print(f"   - {os.path.basename(file)}")
    print()
    
    # ����� Ȯ��
    response = input("����Ͻðڽ��ϱ�? (y/N): ").strip().lower()
    if response not in ['y', 'yes']:
        print("[INFO] ��ҵǾ����ϴ�.")
        return
    
    print("\n[INFO] ���̼��� ��� �߰� ��...")
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
    print("[INFO] ó�� ���:")
    print(f"   [INFO] ����: {success_count}��")
    print(f"   [INFO] �ǳʶ�: {skip_count}��")
    print(f"   [ERROR] ����: {error_count}��")
    print()
    
    if success_count > 0:
        print("[INFO] ��� ������ .backup Ȯ���ڷ� �����Ǿ����ϴ�.")
        print("   ������ ������ ��� ������ �����ϼ���.")
    
    print("[INFO] �Ϸ�!")

if __name__ == "__main__":
    main() 