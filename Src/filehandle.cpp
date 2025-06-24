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

#include "filehandle.h"

SafeFileHandle::SafeFileHandle(const std::wstring& path, std::ios::openmode mode)
    : m_path(path), m_mode(mode) {
    m_file.open(path, mode);
}

SafeFileHandle::~SafeFileHandle() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

bool SafeFileHandle::is_open() const {
    return m_file.is_open();
}

std::ifstream& SafeFileHandle::get() {
    return m_file;
} 