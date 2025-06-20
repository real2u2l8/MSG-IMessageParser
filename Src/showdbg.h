#ifndef __SHOWDEBUG_H__
	#define __SHOWDEBUG_H__

	#include <windef.h>

	#define SHOW_DEBUG_RESULT
	//#define SHOW_DEBUG_JUST_FUNCCHECK_RESULT
	//#define SHOW_CRITICAL_DEBUG_RESULT
	#ifndef _CLIENT_DLL
		//#define _WRITELOG
	#endif


	#define DEBUG_MODULENAME "[IMessageParser] "
	#define LOGFILE_LIMIT_SIZE 5242880 //5 Mbyte
	#define DEBUG_WRITE_PATH "C:\\IMessageParser\\Logs\\IMessageParser.log"

	// 멀티바이트 문자 집합일 때만 사용 가능한 디버그 함수들
	#if !defined(UNICODE) && !defined(_UNICODE)
	extern void _SHOW_DEBUG(char *fmt, ...);
	extern void WriteLOGFile(char *szFilePath, const char *pFmt, ...);
	#else
	// 유니코드 모드일 때는 빈 함수로 정의 (컴파일 에러 방지)
	extern void _SHOW_DEBUG(char *fmt, ...);
	extern void WriteLOGFile(char *szFilePath, const char *pFmt, ...);
	#endif

	// 유니코드 버전은 항상 사용 가능
	extern void _SHOW_DEBUG_W(const WCHAR *fmt, ...);
	extern void WriteLOGFileW(WCHAR *szFilePath, const WCHAR *pFmt, ...);

	//////////////////////////////////////////////////////////////////////////

	#ifdef SHOW_DEBUG_RESULT
	#define SHOW_DEBUG(_x_) _SHOW_DEBUG _x_
	#define SHOW_DEBUG_W(_x_) _SHOW_DEBUG_W _x_
	#define SHOW_DEBUG_FUNCCHECK(_x_)


	#ifdef SHOW_CRITICAL_DEBUG_RESULT
	#define SHOW_CRI_DEBUG(_x_) _SHOW_DEBUG _x_
	#else
	#define SHOW_CRI_DEBUG(_x_)
	#endif

	#elif defined(SHOW_DEBUG_JUST_FUNCCHECK_RESULT)
	#define SHOW_DEBUG(_x_)
	#define SHOW_DEBUG_FUNCCHECK(_x_) _SHOW_DEBUG _x_
	#define SHOW_CRI_DEBUG(_x_)

	#else

	#define SHOW_DEBUG(_x_)
	#define SHOW_DEBUG_FUNCCHECK(_x_)
	#define SHOW_CRI_DEBUG(_x_)
	#endif



#endif //#ifndef __SHOWDEBUG_H__ end