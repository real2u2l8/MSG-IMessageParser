#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
GNU GPL 3.0 License Header Addition Script
MSG File Parser Tool Project

Usage: python add_license_headers.py
"""

import os
import glob
import re

# License header template
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
    """Check if file should be skipped"""
    skip_patterns = [
        'pch',  # Precompiled header
        'resource.h',  # Auto-generated file
        'IMessageParser.rc',  # Resource file
        'bitmap1.bmp',  # Bitmap file
    ]
    
    for pattern in skip_patterns:
        if pattern in filename:
            return True
    return False

def has_license_header(content):
    """Check if file already has license header"""
    return "MSG File Parser Tool - Advanced MSG file analysis tool" in content

def add_license_header(filepath):
    """Add license header to file"""
    try:
        # Try multiple encodings to read file
        content = None
        encodings_to_try = ['utf-8', 'cp949', 'euc-kr', 'latin-1', 'iso-8859-1']
        
        for encoding in encodings_to_try:
            try:
                with open(filepath, 'r', encoding=encoding) as f:
                    content = f.read()
                break
            except UnicodeDecodeError:
                continue
        
        if content is None:
            print(f"[ERROR] Cannot read file (encoding issue): {filepath}")
            return False
        
        # Skip if license header already exists
        if has_license_header(content):
            print(f"[INFO] License header already exists: {filepath}")
            return False
        
        # Add license header at the beginning of file
        new_content = LICENSE_HEADER + content
        
        # Create backup file
        backup_path = filepath + '.backup'
        with open(backup_path, 'w', encoding='utf-8') as f:
            f.write(content)
        
        # Overwrite file with new content
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        
        print(f"[INFO] License header added: {filepath}")
        return True
        
    except Exception as e:
        print(f"[ERROR] Error occurred ({filepath}): {e}")
        return False

def main():
    """Main function"""
    print("[INFO] GNU GPL 3.0 License Header Addition Script")
    print("=" * 50)
    
    # Find .cpp and .h files in Src directory
    src_dir = "Src"
    if not os.path.exists(src_dir):
        print(f"[ERROR] Cannot find {src_dir} directory.")
        return
    
    # File patterns
    patterns = [
        os.path.join(src_dir, "*.cpp"),
        os.path.join(src_dir, "*.h")
    ]
    
    target_files = []
    for pattern in patterns:
        target_files.extend(glob.glob(pattern))
    
    # Filter out files to skip
    target_files = [f for f in target_files if not should_skip_file(f)]
    
    if not target_files:
        print("[INFO] No files to process found.")
        return
    
    print(f"[INFO] Files to process ({len(target_files)}):")
    for file in target_files:
        print(f"   - {os.path.basename(file)}")
    print()
    
    # User confirmation
    response = input("Continue? (y/N): ").strip().lower()
    if response not in ['y', 'yes']:
        print("[INFO] Cancelled.")
        return
    
    print("\n[INFO] Adding license headers...")
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
    print("[INFO] Processing results:")
    print(f"   [INFO] Success: {success_count}")
    print(f"   [INFO] Skipped: {skip_count}")
    print(f"   [ERROR] Errors: {error_count}")
    print()
    
    if success_count > 0:
        print("[INFO] Backup files created with .backup extension.")
        print("   Delete backup files if everything is OK.")
    
    print("[INFO] Complete!")

if __name__ == "__main__":
    main() 