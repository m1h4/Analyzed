#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include "Resource.h"

#include <atlbase.h>
#include <atlcom.h>

#include <windowsx.h>

#include <vector>

#include <d3d9.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <xmmintrin.h>

// Debug help headers
#include "Assert.h"
#include "Trace.h"

// Custom array and string classes
#include "Array.h"
#include "String.h"

// Custom graphics help classes
#include "Color.h"
#include "Point.h"
#include "Rect.h"
#include "Size.h"

// Debug help function for assigning a name to a thread
typedef struct
{
	DWORD dwType;		// Must be 0x1000
	LPCSTR szName;		// Pointer to name (in user address space)
	DWORD dwThreadID;	// Thread ID (-1 = caller thread)
	DWORD dwFlags;		// Reserved for future use, must be zero
} THREADNAMEINFO,*LPTHREADNAMEINFO;

VOID SetThreadName(LPCSTR szThreadName);

#ifdef _DEBUG
#define SETTHREADNAME SetThreadName
#else
#define SETTHREADNAME __noop
#endif

// Interpolation functions
float LinearInterpolation(float p1,float p2,float s);
float CubicInterpolation(float p1,float p2,float s);
float CosInterpolation(float p1,float p2,float s);

//inline void* __cdecl operator new(unsigned int,void* ptr) { return (ptr); }
//inline void __cdecl operator delete(void*,void*) { return; }

#define SAFERELEASE(ptr) {if(ptr) { ptr->Release(); ptr = NULL; }}
#define SAFEDELETE(ptr) {if(ptr) { delete ptr; ptr = NULL; }}

using namespace ATL;