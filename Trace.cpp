#include "Globals.h"

VOID Trace(LPCTSTR fmt,...)
{
	ASSERT(fmt);

	TCHAR buffer[2048];
    
    va_list args;
    va_start(args,fmt);
	wvsprintf(buffer,fmt,args);
    va_end(args);

	OutputDebugString(buffer);
}