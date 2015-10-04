#include "Globals.h"
#include "Assert.h"

#include <signal.h>

BOOL Assert(LPCTSTR expression,LPCTSTR function,LPCTSTR file,LONG line)
{
	TCHAR message[4096];

	// TODO Possible buffer overflows
	wsprintf(message,TEXT("Assertion failed in the following expression:\n\n%s\n\nFunction: %s\nFile: %s\nLine: %d\n\nDo you want to abort the application? Chosing No will break into the debuger."),expression,function,file,line);

	switch(MessageBox(NULL,message,TEXT("Assertion Failed"),MB_YESNOCANCEL|MB_ICONEXCLAMATION|MB_TASKMODAL))
	{
	case IDYES:
		raise(SIGABRT);
		exit(3);

	default:
	case IDNO:
		return TRUE;

	case IDCANCEL:
		return FALSE;
	}
}