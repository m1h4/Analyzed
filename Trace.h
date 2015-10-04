#pragma once

#ifdef _DEBUG
#define TRACE Trace
#else
#define TRACE __noop
#endif

VOID Trace(LPCTSTR fmt,...);