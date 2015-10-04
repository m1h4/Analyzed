#pragma once

#include "Analyzed_h.h"
#include "Effects.h"
#include "Graphics.h"

class ATL_NO_VTABLE CAnalyzed :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAnalyzed,&CLSID_Analyzed>,
	public IDispatchImpl<IAnalyzed, &IID_IAnalyzed, &LIBID_Analyzed>,
	public IWMPEffects2
{
public:
	CAnalyzed() {}
	~CAnalyzed() {}

	DECLARE_REGISTRY_RESOURCEID(IDR_ANALYZED)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CAnalyzed)
		COM_INTERFACE_ENTRY(IWMPEffects)
		COM_INTERFACE_ENTRY(IWMPEffects2)
		COM_INTERFACE_ENTRY(IAnalyzed)
	END_COM_MAP()

public:
	// CComCoClass Overrides
    HRESULT FinalConstruct(VOID);
    VOID FinalRelease(VOID);

    // IWMPEffects
	STDMETHOD(Render)(LPTIMEDLEVEL pLevels,HDC hdc,RECT* rc) { return S_OK; }
	STDMETHOD(MediaInfo)(LONG lChannelCount,LONG lSampleRate,BSTR bstrTitle) { return S_OK; }
    STDMETHOD(GetCapabilities)(DWORD* pdwCapabilities);
    STDMETHOD(GoFullscreen)(BOOL fFullScreen) { return E_NOTIMPL; }
    STDMETHOD(RenderFullScreen)(LPTIMEDLEVEL pLevels) { return E_NOTIMPL; }
    STDMETHOD(DisplayPropertyPage)(HWND hwndOwner);
    STDMETHOD(GetTitle)(BSTR* bstrTitle);
    STDMETHOD(GetPresetTitle)(LONG nPreset,BSTR* bstrPresetTitle);
    STDMETHOD(GetPresetCount)(LONG* pnPresetCount);
    STDMETHOD(SetCurrentPreset)(LONG nPreset);
    STDMETHOD(GetCurrentPreset)(LONG* pnPreset);

    // IWMPEffects2
    STDMETHOD(SetCore)(IWMPCore* pCore);
    STDMETHOD(Create)(HWND hwndParent);
    STDMETHOD(Destroy)(VOID);
	STDMETHOD(NotifyNewMedia)(IWMPMedia* pMedia) { return S_OK; }
	STDMETHOD(OnWindowMessage)(UINT msg,WPARAM WParam,LPARAM LParam,LRESULT* plResultParam) { return S_FALSE; }
    STDMETHOD(RenderWindowed)(LPTIMEDLEVEL pLevels,BOOL fRequiredRender);

	// IAnalyzed
	STDMETHOD(Initialize)(OLE_HANDLE window,LONG resolution,VARIANT_BOOL blur);
	STDMETHOD(Uninitialize)(void);
	STDMETHOD(GetVisualization)(LONG* index);
	STDMETHOD(SetVisualization)(LONG index);
	STDMETHOD(GetVisualizationCount)(LONG* count);
	STDMETHOD(GetVisualizationName)(BSTR* name, LONG index);
	STDMETHOD(Update)(OLE_HANDLE levels);
	STDMETHOD(GetColorBackground)(LONG* color);
	STDMETHOD(SetColorBackground)(LONG color);
	STDMETHOD(GetColorTop)(LONG* color);
	STDMETHOD(SetColorTop)(LONG color);
	STDMETHOD(GetColorTopEnd)(LONG* color);
	STDMETHOD(SetColorTopEnd)(LONG color);
	STDMETHOD(GetColorBottom)(LONG* color);
	STDMETHOD(SetColorBottom)(LONG color);
	STDMETHOD(GetColorBottomEnd)(LONG* color);
	STDMETHOD(SetColorBottomEnd)(LONG color);
	STDMETHOD(GetColorPeak)(LONG* color);
	STDMETHOD(SetColorPeak)(LONG color);

private:
    CComPtr<IWMPCore>	m_Core;
	CGraphics			m_Graphics;
};

OBJECT_ENTRY_AUTO(CLSID_Analyzed,CAnalyzed)
