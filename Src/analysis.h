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
#include "msgparser.h"
#include "mapiparser.h"
#include "filehandle.h"

// MSG 파일 내부 스토리지/스트림 이름 정의
#define		SUBSTG_SENDER		L"__substg1.0_0C1F"		// 발신자 정보
#define		SUBSTG_TO			L"__substg1.0_0E04"		// 수신자 (TO)
#define		SUBSTG_CC			L"__substg1.0_0E03"		// 참조 (CC)
#define		SUBSTG_BCC			L"__substg1.0_0E02"		// 숨은참조 (BCC)
#define		SUBSTG_SUBJECT		L"__substg1.0_0037"		// 제목
#define		SUBSTG_BODY			L"__substg1.0_1000"		// 본문 (텍스트)
#define		SUBSTG_HTML_BODY	L"__substg1.0_1013"		// 본문 (HTML)
#define		SUBSTG_RTF_BODY		L"__substg1.0_1009"		// 본문 (RTF)
#define		SUBSTG_ATTACHMENTS	L"__attach_"				// 첨부파일
#define		SUBSTG_RECIPIENTS	L"__recip_version1.0_"	// 수신자 목록
#define		SUBSTG_PROPERTIES	L"__properties_version1.0"	// MAPI 속성

// MAPI 속성 태그 정의
#define		PR_RECIPIENT_TYPE	0x0C15		// 수신자 타입
#define		PR_DISPLAY_NAME		0x3001		// 표시 이름
#define		PR_SMTP_ADDRESS		0x5D01		// SMTP 주소
#define		PR_EMAIL_ADDRESS	0x3003		// 이메일 주소
#define		PR_ADDRTYPE			0x3002		// 주소 타입
#define		PR_ENTRYID			0x0FFF		// 항목 ID
#define		PR_SEARCH_KEY		0x300B		// 검색 키

// MAPI 속성 타입 정의
#define		PT_UNSPECIFIED		0x0000		// 미지정
#define		PT_NULL				0x0001		// NULL
#define		PT_I2				0x0002		// 16비트 정수
#define		PT_LONG				0x0003		// 32비트 정수
#define		PT_R4				0x0004		// 32비트 실수
#define		PT_DOUBLE			0x0005		// 64비트 실수
#define		PT_CURRENCY			0x0006		// 통화
#define		PT_APPTIME			0x0007		// 시간
#define		PT_ERROR			0x000A		// 오류
#define		PT_BOOLEAN			0x000B		// 불린
#define		PT_OBJECT			0x000D		// 객체
#define		PT_I8				0x0014		// 64비트 정수
#define		PT_STRING8			0x001E		// 8비트 문자열
#define		PT_UNICODE			0x001F		// 유니코드 문자열
#define		PT_SYSTIME			0x0040		// 시스템 시간
#define		PT_CLSID			0x0048		// GUID
#define		PT_BINARY			0x0102		// 바이너리 데이터

// 첨부파일 관련 스토리지 이름
#define		SUBSTG_ATTACHDISPLAY	L"__substg1.0_3001"		// 첨부파일 표시명
#define		SUBSTG_ATTACHNAME		L"__substg1.0_3707"		// 첨부파일 이름
#define		SUBSTG_ATTACHDATA		L"__substg1.0_3701"		// 첨부파일 데이터

// 주소 관련 스토리지 이름
#define		SUBSTG_ADDRTYPE		    L"__substg1.0_3002"		// 주소 타입
#define		SUBSTG_SENDER_ADDRTYPE	L"__substg1.0_0C1E"		// 발신자 주소 타입
#define		SUBSTG_SMTPADDRESS		L"__substg1.0_39FE"		// SMTP 주소
#define		SUBSTG_SUBTO		    L"__substg1.0_3003"		// 수신자 주소
#define		SUBSTG_RECIPIENT_TYPE	L"__substg1.0_0C15"		// 수신자 타입

// 수신자 타입 상수
#define		RECIP_TYPE_TO			1		// 수신자 (TO)
#define		RECIP_TYPE_CC			2		// 참조 (CC)
#define		RECIP_TYPE_BCC			3		// 숨은참조 (BCC)

// 최대 수신자 주소 길이
#define		MAX_RECIPIENT_ADDR_LEN	256