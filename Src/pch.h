//
// 프로젝트 최상위 헤더 (pch.h)
// 원칙:
// 1. 자주 사용되는 헤더들만 포함
// 2. 변경이 적은 헤더들만 포함
// 3. 프로젝트 전체에서 공통으로 사용되는 헤더들만 포함
// 4. 특정 기능에만 사용되는 헤더는 포함하지 않음

#pragma once

// Windows OLE/COM related headers
#include <windows.h>
#include <ole2.h>
#include <oleauto.h>
#include <objidl.h>
#include <objbase.h>
#include <comdef.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <fcntl.h>
#include <io.h>

// Standard C++ headers
// Containers
#include <map>
#include <string>
#include <vector>

// I/O Streams
#include <fstream>
#include <iostream>
#include <sstream>

// Utilities
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <memory>

// C Standard Library
#include <stdarg.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>

// Additional Windows headers for file operations
#include <shlobj.h>
#include <shlwapi.h>
#include <winreg.h>
#include <tchar.h>
#include <io.h>

// For Unicode support
#include <locale>

// Project Header
#include "showdbg.h"
