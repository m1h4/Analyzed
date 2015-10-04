#pragma once

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);

	void Begin(LPCTSTR name);
	void End(double elapsed);

	void BeginSection(LPCTSTR name);
	void EndSection(void);

protected:
	std::vector<long long>	m_Times;
	std::vector<LPCTSTR>	m_Names;
	LPCTSTR					m_Name;
};
