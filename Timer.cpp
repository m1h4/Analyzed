#include "Globals.h"
#include "Timer.h"

CTimer::CTimer(void)
{
}

CTimer::~CTimer(void)
{
}

void CTimer::Begin(LPCTSTR name)
{
	m_Name = name;

	m_Times.empty();
}

void CTimer::End(double elapsed)
{
	static double time = 0.0;

	time += elapsed;

	if(time > 50.0)
	{
		time = 0.0;

		if(!m_Times.size())
			return;

		long long total = 0;

		for(UINT i = 0; i < m_Times.size(); i += 2)
			total += m_Times[i+1] - m_Times[i];

		OutputDebugString(TEXT("Begin "));
		OutputDebugString(m_Name);
		OutputDebugString(TEXT(":\n"));

		TCHAR string[0x100];

		for(UINT i = 0; i < m_Times.size(); i += 2)
		{
			_stprintf(string,TEXT("  %s: %f%%\n"),m_Names[i/2],double(m_Times[i+1] - m_Times[i]) / double(total) * 100.0);
			OutputDebugString(string);
		}

		OutputDebugString(TEXT("End "));
		OutputDebugString(m_Name);
		OutputDebugString(TEXT(".\n"));
	}

	m_Times.empty();
}

void CTimer::BeginSection(LPCTSTR name)
{
	long long time;

	QueryPerformanceCounter((PLARGE_INTEGER)&time);

	m_Times.push_back(time);
	m_Names.push_back(name);
}

void CTimer::EndSection(void)
{
	long long time;

	QueryPerformanceCounter((PLARGE_INTEGER)&time);

	m_Times.push_back(time);
}