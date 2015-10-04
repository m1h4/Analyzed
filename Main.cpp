#include "Globals.h"
#include "Resource.h"
#include "Analyzed_i.c"
#include "WmpPlug.h"

CComModule _Module;

EXTERN_C BOOL WINAPI DllMain(HINSTANCE hInstance,DWORD dwReason,LPVOID lpReserved)
{
    return _Module.DllMain(hInstance,dwReason,lpReserved,NULL,NULL); 
}

STDAPI DllCanUnloadNow(VOID)
{
    return _Module.DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid,REFIID riid,LPVOID* ppv)
{
    return _Module.DllGetClassObject(rclsid,riid,ppv);
}

STDAPI DllRegisterServer(VOID)
{
	HRESULT hResult = _Module.DllRegisterServer(FALSE);

	WMPNotifyPluginAddRemove();

	return hResult;
}

STDAPI DllUnregisterServer(VOID)
{
	HRESULT hResult = _Module.DllUnregisterServer(FALSE);

	WMPNotifyPluginAddRemove();

	return hResult;
}
