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

//////////////////////////////////////////////////////////////////////////
// 멀티바이트 문자 세트일 때만 사용 가능한 디버그 함수들
//////////////////////////////////////////////////////////////////////////
#if !defined(UNICODE) && !defined(_UNICODE)

void WriteLOGFile(char *szFilePath, const char *pFmt, ...)
{
	WIN32_FILE_ATTRIBUTE_DATA Attribute;
	if (::GetFileAttributesExA(szFilePath, GetFileExInfoStandard, &Attribute) == FALSE)
		Attribute.nFileSizeLow = 0;

	if (Attribute.nFileSizeLow > LOGFILE_LIMIT_SIZE)
		unlink(szFilePath);

	char szlogdata[2000] = { 0 };
	char szdate[100] = { 0 };
	char sz_Msg[20480] = { 0, };

	va_list argList;
	time_t tsys;
	struct tm *ptm = NULL;
	FILE *ff = NULL;
	HANDLE hFile = NULL;
	DWORD dwWriteSize = 0;

	va_start(argList, pFmt);
	vsprintf(szlogdata, pFmt, argList);
	va_end(argList);

	time(&tsys);
	if ((ptm = localtime(&tsys)) != NULL)
	{
		sprintf(szdate, "%d-%02d-%02d %02d:%02d:%02d- ",
			ptm->tm_year + 1900,
			ptm->tm_mon + 1,
			ptm->tm_mday,
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_sec);
	}

	hFile = CreateFileA(szFilePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		char szFileDirPath[MAX_PATH] = { 0, };
		strcpy_s(szFileDirPath, MAX_PATH, szFilePath);
		char *pIndex = strrchr(szFileDirPath, '\\');
		if (pIndex)
		{
			*pIndex = '\0';
		}
		CreateDirectoryA(szFileDirPath, NULL);
	}

	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile, 0, 0, FILE_END);

		sprintf(sz_Msg, "%s %s\r\n", szdate, szlogdata);

		dwWriteSize = 0;
		WriteFile(hFile,
			sz_Msg,
			sizeof(char) * strlen(sz_Msg),
			&dwWriteSize,
			NULL);

		CloseHandle(hFile);
		hFile = NULL;
	}
}

void _SHOW_DEBUG(char *fmt, ...)
{
	char msg[20480] = DEBUG_MODULENAME;
	va_list vargs;
	//	OutputDebugString(fmt);
	va_start(vargs, fmt);
	_vsntprintf(msg + strlen(msg), sizeof(msg), fmt, vargs);
	va_end(vargs);

	//	#ifdef LINE_FEED_ATTACH
	strcat(msg, "\n");
	//	#endif

	OutputDebugString(msg);

#if (defined(_WRITELOG) && defined(DEBUG_WRITE_PATH))
	WriteLOGFile(DEBUG_WRITE_PATH, "%s", msg);
#endif
}

#else
//////////////////////////////////////////////////////////////////////////
// 유니코드 빌드일 때는 빈 함수로 정의 (컴파일 에러 방지)
//////////////////////////////////////////////////////////////////////////
void WriteLOGFile(char *szFilePath, const char *pFmt, ...) {
	// 멀티바이트 빌드에서만 사용 가능
}

void _SHOW_DEBUG(char *fmt, ...) {
	// 멀티바이트 빌드에서만 사용 가능
}
#endif

//////////////////////////////////////////////////////////////////////////
// 유니코드 버전 디버그/로그 함수들
//////////////////////////////////////////////////////////////////////////
void _SHOW_DEBUG_W(const WCHAR *fmt, ...)
{
	WCHAR msg[4096] = { 0 };
	va_list vargs;

	swprintf(msg, L"[%S] ", DEBUG_MODULENAME);

	va_start(vargs, fmt);
	_vsnwprintf(msg + wcslen(msg), sizeof(msg), fmt, vargs);
	va_end(vargs);

	wcscat(msg, L"\n");

	OutputDebugStringW(msg);
}

void WriteLOGFileW(WCHAR *szFilePath, const WCHAR *pFmt, ...)
{
	WIN32_FILE_ATTRIBUTE_DATA Attribute;
	if (::GetFileAttributesExW(szFilePath, GetFileExInfoStandard, &Attribute) == FALSE)
		Attribute.nFileSizeLow = 0;

	if (Attribute.nFileSizeLow > LOGFILE_LIMIT_SIZE)
		_wunlink(szFilePath);

	WCHAR szlogdata[2000] = { 0 };
	WCHAR szdate[100] = { 0 };
	WCHAR sz_Msg[20480] = { 0, };

	va_list argList;
	time_t tsys;
	struct tm *ptm = NULL;
	FILE *ff = NULL;
	HANDLE hFile = NULL;
	DWORD dwWriteSize = 0;

	va_start(argList, pFmt);
	vswprintf(szlogdata, 2000, pFmt, argList);
	va_end(argList);

	time(&tsys);
	if ((ptm = localtime(&tsys)) != NULL)
	{
		swprintf(szdate, 100, L"%d-%02d-%02d %02d:%02d:%02d- ",
			ptm->tm_year + 1900,
			ptm->tm_mon + 1,
			ptm->tm_mday,
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_sec);
	}

	hFile = CreateFileW(szFilePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		WCHAR szFileDirPath[MAX_PATH] = { 0, };
		wcscpy_s(szFileDirPath, MAX_PATH, szFilePath);
		WCHAR *pIndex = wcsrchr(szFileDirPath, L'\\');
		if (pIndex)
		{
			*pIndex = L'\0';
		}
		CreateDirectoryW(szFileDirPath, NULL);
	}

	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile, 0, 0, FILE_END);

		swprintf(sz_Msg, 20480, L"%s %s\r\n", szdate, szlogdata);

		dwWriteSize = 0;
		WriteFile(hFile,
			sz_Msg,
			sizeof(WCHAR) * wcslen(sz_Msg),
			&dwWriteSize,
			NULL);

		CloseHandle(hFile);
		hFile = NULL;
	}
}
