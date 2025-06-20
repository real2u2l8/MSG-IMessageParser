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

	// ��Ƽ����Ʈ ���� ������ ���� ��� ������ ����� �Լ���
	#if !defined(UNICODE) && !defined(_UNICODE)
	extern void _SHOW_DEBUG(char *fmt, ...);
	extern void WriteLOGFile(char *szFilePath, const char *pFmt, ...);
	#else
	// �����ڵ� ����� ���� �� �Լ��� ���� (������ ���� ����)
	extern void _SHOW_DEBUG(char *fmt, ...);
	extern void WriteLOGFile(char *szFilePath, const char *pFmt, ...);
	#endif

	// �����ڵ� ������ �׻� ��� ����
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