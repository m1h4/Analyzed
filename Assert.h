#pragma once

#ifdef _DEBUG
#define ASSERT(exp) if(!(exp)) { if(Assert(TEXT(#exp),TEXT(__FUNCTION__),TEXT(__FILE__),__LINE__)) DebugBreak(); }
#else
#define ASSERT(exp) __noop
#endif

BOOL Assert(LPCTSTR expression,LPCTSTR function,LPCTSTR file,LONG line);