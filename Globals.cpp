#include "Globals.h"

float LinearInterpolation(float p1,float p2,float s)
{
	return p1 + s * (p2 - p1);
}

float CubicInterpolation(float p1,float p2,float s)
{
	float s1 = 2.0f * powf(s,3.0f);
	float s2 = 3.0f * powf(s,2.0f);

	return p1 * (s1 - s2 + 1.0f) + p2 * (s2 - s1);
}

float CosInterpolation(float p1,float p2,float s)
{
	float f1 = (1.0f - cosf((float)M_PI * s)) * 0.5f;

	return  p1 * (1.0f - f1) + p2 * f1;
}

VOID SetThreadName(LPCSTR szThreadName )
{
	THREADNAMEINFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = -1;
	info.dwFlags = 0;

	__try
	{
		RaiseException(0x406D1388,0,sizeof(info)/sizeof(DWORD),(LPDWORD)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}
