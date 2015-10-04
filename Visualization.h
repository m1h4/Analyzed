#pragma once

extern CComModule _Module;

class CVisualization
{
public:
	CVisualization(UINT nameid) { SetName(nameid); }
	virtual ~CVisualization(VOID) {}

	LPCTSTR GetName(VOID) const { return m_Name; }

	virtual HRESULT Draw(class CGraphics& graphics) = NULL;

protected:
	TCHAR	m_Name[512];

protected:
	HRESULT SetName(UINT id)
	{ 
		LoadString(_Module.GetModuleInstance(),id,m_Name,sizeof(m_Name)/sizeof(TCHAR));
		return S_OK;
	}
};