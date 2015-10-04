#include "Globals.h"
#include "Analyzed.h"
#include "Visualizations.h"

INT_PTR WINAPI StaticDialogProcedure(HWND hDialog,UINT uMessage,WPARAM wParam,LPARAM lParam);

HRESULT CAnalyzed::FinalConstruct()
{
	HRESULT hr = m_Graphics.Initialize();
	if(FAILED(hr))
		return hr;

	m_Graphics.AddVisualization(new CBars);
	m_Graphics.AddVisualization(new CDoubleBars);
	m_Graphics.AddVisualization(new CDigitalBars);
	m_Graphics.AddVisualization(new CGhostBars);
	m_Graphics.AddVisualization(new CDoubleGhostBars);
	m_Graphics.AddVisualization(new CDigitalGhostBars);
	m_Graphics.AddVisualization(new CBarScope);
	m_Graphics.AddVisualization(new CDigitalScope);

    return hr;
}

VOID CAnalyzed::FinalRelease()
{
	m_Graphics.Uninitialize();
	
	m_Core = NULL;
}

STDMETHODIMP CAnalyzed::GetCapabilities(DWORD* pdwCapabilities)
{
    if(!pdwCapabilities)
        return E_POINTER;

	*pdwCapabilities = EFFECT_HASPROPERTYPAGE;

    return S_OK;
}

STDMETHODIMP CAnalyzed::DisplayPropertyPage(HWND hwndOwner)
{
	PROPSHEETPAGE psp;
	ZeroMemory(&psp,sizeof(PROPSHEETPAGE));

    psp.dwSize = sizeof(PROPSHEETPAGE);
    psp.dwFlags = PSP_DEFAULT;
	psp.hInstance = _Module.GetModuleInstance();
	psp.pszTemplate = MAKEINTRESOURCE(IDD_PROPPAGE_MEDIUM);
    psp.pfnDlgProc = StaticDialogProcedure;
    psp.lParam = (LPARAM)this;

	HPROPSHEETPAGE rhPage = CreatePropertySheetPage(&psp);
	if(!rhPage)
		return E_FAIL;

    PROPSHEETHEADER psh;
    psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_DEFAULT|PSH_PROPTITLE|PSH_NOAPPLYNOW;
    psh.hwndParent = hwndOwner;
    psh.hInstance = _Module.GetModuleInstance();
    psh.pszCaption = NULL;
    psh.nPages = 1;
    psh.phpage = &rhPage; 

    // Display the property sheet
    PropertySheet(&psh);

    return S_OK;
}

STDMETHODIMP CAnalyzed::GetTitle(BSTR* bstrTitle)
{
    if(!bstrTitle)
        return E_POINTER;

    CComBSTR bstrTemp;
    bstrTemp.LoadString(IDS_EFFECTNAME); 
        
    if(!bstrTemp || !bstrTemp.Length())
        return E_FAIL;

    *bstrTitle = bstrTemp.Detach();

    return S_OK;
}

STDMETHODIMP CAnalyzed::GetPresetTitle(LONG nPreset, BSTR *bstrPresetTitle)
{
    if(!bstrPresetTitle)
        return E_POINTER;

	// Check for overflow
	if(nPreset < 0 || nPreset > (LONG)m_Graphics.GetVisualizationCount() - 1)
		return E_INVALIDARG;

	// Get the name
	CComBSTR bstrTemp(m_Graphics.GetVisualization(nPreset)->GetName());

    *bstrPresetTitle = bstrTemp.Detach();

    return S_OK;
}

STDMETHODIMP CAnalyzed::GetPresetCount(LONG* pnPresetCount)
{
    if(!pnPresetCount)
        return E_POINTER;

	*pnPresetCount = m_Graphics.GetVisualizationCount();

    return S_OK;
}

STDMETHODIMP CAnalyzed::SetCurrentPreset(LONG nPreset)
{
	if(FAILED(m_Graphics.SetVisualization(nPreset)))
        return E_INVALIDARG;

    return S_OK;
}

STDMETHODIMP CAnalyzed::GetCurrentPreset(LONG* pnPreset)
{
    if(!pnPreset)
        return E_POINTER;

	*pnPreset = m_Graphics.GetVisualization();

    return S_OK;
}

STDMETHODIMP CAnalyzed::SetCore(IWMPCore* pCore)
{
    // Set our core
    m_Core = pCore;

    return S_OK;
}

VOID ParseResolution(ULONG resolution,PULONG width,PULONG height)
{
	ASSERT(width);
	ASSERT(height);

	switch(resolution)
	{
	case 0:
		*width = 256;
		*height = 192;
		break;

	case 1:
		*width = 384;
		*height = 288;
		break;

	default:
	case 2:
		*width = 512;
		*height = 384;
		break;

	case 3:
		*width = 1024;
		*height = 768;
		break;

	case 4:
		CRegKey regKey;
		regKey.Open(HKEY_CURRENT_USER,TEXT("Software\\Miha Software\\Analyzed Visualization"),KEY_READ);
		regKey.QueryDWORDValue(TEXT("CustomWidth"),*width);
		regKey.QueryDWORDValue(TEXT("CustomHeight"),*height);
		regKey.Close();
		break;
	}
}

#define UNPACK_COLOR(color) Color(((color) >> 24)/255.0f,(((color) >> 16) & 0xFF)/255.0f,(((color) >> 8) & 0xFF)/255.0f,((color) & 0xFF)/255.0f)

STDMETHODIMP CAnalyzed::Create(HWND hwndParent)
{
	HRESULT hr = m_Graphics.InitializeTimer();
	if(FAILED(hr))
		return hr;

	ULONG resolution = 2;
	ULONG blur = 1;

	CRegKey regKey;
	regKey.Open(HKEY_CURRENT_USER,TEXT("Software\\Miha Software\\Analyzed Visualization"),KEY_READ);
	regKey.QueryDWORDValue(TEXT("Resolution"),resolution);
	regKey.QueryDWORDValue(TEXT("Blur"),blur);
	regKey.Close();

	ULONG width;
	ULONG height;

	ParseResolution(resolution,&width,&height);

	hr = m_Graphics.InitializeDisplay(hwndParent,width,height,blur > 0 ? TRUE : FALSE);
	if(FAILED(hr))
		return hr;

	hr = m_Graphics.InitializeRenderer();
	if(FAILED(hr))
		return hr;

	ULONG top = m_Graphics.GetColorTop();
	ULONG topEnd = m_Graphics.GetColorTopEnd();
	ULONG bottom = m_Graphics.GetColorBottom();
	ULONG bottomEnd = m_Graphics.GetColorBottomEnd();
	ULONG background = m_Graphics.GetColorBackground();
	ULONG peak = m_Graphics.GetColorPeak();

	regKey.Open(HKEY_CURRENT_USER,TEXT("Software\\Miha Software\\Analyzed Visualization"),KEY_READ);
	regKey.QueryDWORDValue(TEXT("ColorTop"),top);
	regKey.QueryDWORDValue(TEXT("ColorTopEnd"),topEnd);
	regKey.QueryDWORDValue(TEXT("ColorBottom"),bottom);
	regKey.QueryDWORDValue(TEXT("ColorBottomEnd"),bottomEnd);
	regKey.QueryDWORDValue(TEXT("ColorBackground"),background);
	regKey.QueryDWORDValue(TEXT("ColorPeak"),peak);
	regKey.Close();

	m_Graphics.SetColorTop(UNPACK_COLOR(top));
	m_Graphics.SetColorTopEnd(UNPACK_COLOR(topEnd));
	m_Graphics.SetColorBottom(UNPACK_COLOR(bottom));
	m_Graphics.SetColorBottomEnd(UNPACK_COLOR(bottomEnd));
	m_Graphics.SetColorBackground(UNPACK_COLOR(background));
	m_Graphics.SetColorPeak(UNPACK_COLOR(peak));

    return S_OK;
}

STDMETHODIMP CAnalyzed::Destroy()
{
	m_Graphics.UninitializeRenderer();

	m_Graphics.UninitializeDisplay();
	m_Graphics.UninitializeTimer();

    return S_OK;
}

STDMETHODIMP CAnalyzed::RenderWindowed(LPTIMEDLEVEL pLevels,BOOL fRequiredRender)
{
	// Update the graphics levels
	m_Graphics.UpdateLevels(pLevels);

    return S_OK;
}

VOID ParseColorPreset(PULONG preset,PULONG top,PULONG topEnd,PULONG bottom,PULONG bottomEnd,PULONG background,PULONG peak)
{
	ASSERT(preset);
	ASSERT(top);
	ASSERT(topEnd);
	ASSERT(bottom);
	ASSERT(bottomEnd);
	ASSERT(background);
	ASSERT(peak);

	if(*preset == -2)
		return;

	ULONG ptop[3] =			{DEFAULT_COLOR_TOP,			0xffeb2121,	0xff20a42d};
	ULONG ptopEnd[3] =		{DEFAULT_COLOR_TOPEND,		0xfffa5151,	0xff0e971b};
	ULONG pbottom[3] =		{DEFAULT_COLOR_BOTTOM,		0xffa42929,	0xff39a843};
	ULONG pbottomEnd[3] =	{DEFAULT_COLOR_BOTTOMEND,	0xffad4444,	0xff23842c};
	ULONG pbackground[3] =	{DEFAULT_COLOR_BACKGROUND,	0xff000000,	0xff000000};
	ULONG ppeak[3] =		{DEFAULT_COLOR_PEAK,		0xffff2211,	0xff22ff11};

	if(*preset == -1)
	{
		*preset = -2;

		ULONG i;

		for(i = 0; i < sizeof(ptop)/sizeof(ptop[0]); ++i)
			if(ptop[i] == *top)
				break;

		if(i == sizeof(ptop)/sizeof(ptop[0]))
			return;

		if(ptopEnd[i] != *topEnd || pbottom[i] != *bottom || pbottomEnd[i] != *bottomEnd || pbackground[i] != *background || ppeak[i] != *peak)
			return;

		*preset = i;
	}
	else
	{
		if(*preset >= sizeof(ptop)/sizeof(ptop[0]))
			*preset = 0;

		*top = ptop[*preset];
		*topEnd = ptopEnd[*preset];
		*bottom = pbottom[*preset];
		*bottomEnd = pbottomEnd[*preset];
		*background = pbackground[*preset];
		*peak = ppeak[*preset];
	}
}

INT_PTR WINAPI StaticDialogProcedure(HWND hDialog,UINT uMessage,WPARAM wParam,LPARAM lParam)
{
    switch(uMessage) 
    {
        case WM_NOTIFY:
        {
            LPNMHDR pnmh = (LPNMHDR)lParam;
            switch(pnmh->code)
            {
                case PSN_SETACTIVE:
                {
                    // Initialize the values
					ComboBox_AddString(GetDlgItem(hDialog,IDC_RESOLUTION),TEXT("256 by 192 pixels"));
					ComboBox_AddString(GetDlgItem(hDialog,IDC_RESOLUTION),TEXT("384 by 288 pixels"));
					ComboBox_AddString(GetDlgItem(hDialog,IDC_RESOLUTION),TEXT("512 by 384 pixels (Default)"));
					ComboBox_AddString(GetDlgItem(hDialog,IDC_RESOLUTION),TEXT("1024 by 768 pixels"));

					ComboBox_AddString(GetDlgItem(hDialog,IDC_COLOR),TEXT("Blue (Default)"));
					ComboBox_AddString(GetDlgItem(hDialog,IDC_COLOR),TEXT("Red"));
					ComboBox_AddString(GetDlgItem(hDialog,IDC_COLOR),TEXT("Green"));

					ULONG resolution = 1;
					ULONG blur = 1;
					ULONG top,topEnd,bottom,bottomEnd,background,peak;
					ULONG preset = -1;

					CRegKey regKey;
					regKey.Open(HKEY_CURRENT_USER,TEXT("Software\\Miha Software\\Analyzed Visualization"),KEY_READ);
					regKey.QueryDWORDValue(TEXT("Resolution"),resolution);
					regKey.QueryDWORDValue(TEXT("Blur"),blur);
					if(regKey.QueryDWORDValue(TEXT("ColorTop"),top) != ERROR_SUCCESS)
						--preset;
					if(regKey.QueryDWORDValue(TEXT("ColorTopEnd"),topEnd) != ERROR_SUCCESS)
						--preset;
					if(regKey.QueryDWORDValue(TEXT("ColorBottom"),bottom) != ERROR_SUCCESS)
						--preset;
					if(regKey.QueryDWORDValue(TEXT("ColorBottomEnd"),bottomEnd) != ERROR_SUCCESS)
						--preset;
					if(regKey.QueryDWORDValue(TEXT("ColorBackground"),background) != ERROR_SUCCESS)
						--preset;
					if(regKey.QueryDWORDValue(TEXT("ColorPeak"),peak) != ERROR_SUCCESS)
						--preset;
					regKey.Close();

					if(preset == -7)
					{
						ComboBox_SetCurSel(GetDlgItem(hDialog,IDC_COLOR),0);
					}
					else if(preset > -7 && preset < -1)
					{
						ComboBox_AddString(GetDlgItem(hDialog,IDC_COLOR),TEXT("Custom"));
						ComboBox_SetCurSel(GetDlgItem(hDialog,IDC_COLOR),3);	// TODO Change number if changed preset count
					}
					else
					{
						ParseColorPreset(&preset,&top,&topEnd,&bottom,&bottomEnd,&background,&peak);

						if(preset == -2)
						{
							ComboBox_AddString(GetDlgItem(hDialog,IDC_COLOR),TEXT("Custom"));
							ComboBox_SetCurSel(GetDlgItem(hDialog,IDC_COLOR),3);	// TODO Change number if changed preset count
						}
						else
						{
							ComboBox_SetCurSel(GetDlgItem(hDialog,IDC_COLOR),preset);
						}
					}

					if(resolution == 4)
						ComboBox_AddString(GetDlgItem(hDialog,IDC_RESOLUTION),TEXT("Custom"));
					else if(resolution > 4)
							resolution = 1;

					ComboBox_SetCurSel(GetDlgItem(hDialog,IDC_RESOLUTION),resolution);

					CheckDlgButton(hDialog,IDC_BLUR,blur > 0 ? BST_CHECKED : BST_UNCHECKED);

                    return TRUE;
                }

                case PSN_APPLY:
                {
                    // Apply the user's settings

					CRegKey regKey;
					regKey.Create(HKEY_CURRENT_USER,TEXT("Software\\Miha Software\\Analyzed Visualization"));
					regKey.SetDWORDValue(TEXT("Resolution"),ComboBox_GetCurSel(GetDlgItem(hDialog,IDC_RESOLUTION)));
					regKey.SetDWORDValue(TEXT("Blur"),IsDlgButtonChecked(hDialog,IDC_BLUR) == BST_CHECKED);
					
					ULONG preset = ComboBox_GetCurSel(GetDlgItem(hDialog,IDC_COLOR));
					if(preset != 3)	// TODO Change number if changed preset count
					{
						ULONG top,topEnd,bottom,bottomEnd,background,peak;
						ParseColorPreset(&preset,&top,&topEnd,&bottom,&bottomEnd,&background,&peak);

						regKey.SetDWORDValue(TEXT("ColorTop"),top);
						regKey.SetDWORDValue(TEXT("ColorTopEnd"),topEnd);
						regKey.SetDWORDValue(TEXT("ColorBottom"),bottom);
						regKey.SetDWORDValue(TEXT("ColorBottomEnd"),bottomEnd);
						regKey.SetDWORDValue(TEXT("ColorBackground"),background);
						regKey.SetDWORDValue(TEXT("ColorPeak"),peak);
					}

					regKey.Close();

                    return TRUE;
                }
            }
            break;
        }
    }

    return FALSE;
}

STDMETHODIMP CAnalyzed::Initialize(OLE_HANDLE window,LONG resolution,VARIANT_BOOL blur)
{
	if(!window)
		return E_POINTER;

	HRESULT hr = m_Graphics.InitializeTimer();
	if(FAILED(hr))
		return hr;

	ULONG width;
	ULONG height;

	ParseResolution(resolution,&width,&height);

	hr = m_Graphics.InitializeDisplay((HWND)window,width,height,blur);
	if(FAILED(hr))
		return hr;

	hr = m_Graphics.InitializeRenderer();
	if(FAILED(hr))
		return hr;

    return S_OK;
}

STDMETHODIMP CAnalyzed::Uninitialize(void)
{
	m_Graphics.UninitializeRenderer();

	m_Graphics.UninitializeDisplay();
	m_Graphics.UninitializeTimer();

	return S_OK;
}

STDMETHODIMP CAnalyzed::GetVisualization(LONG* index)
{
	if(!index)
		return E_POINTER;

	return GetCurrentPreset(index);
}

STDMETHODIMP CAnalyzed::SetVisualization(LONG index)
{
	return SetCurrentPreset(index);
}

STDMETHODIMP CAnalyzed::GetVisualizationCount(LONG* count)
{
	if(!count)
		return E_POINTER;

	return GetPresetCount(count);
}

STDMETHODIMP CAnalyzed::GetVisualizationName(BSTR* name, LONG index)
{
	return GetPresetTitle(index,name);
}

STDMETHODIMP CAnalyzed::Update(OLE_HANDLE levels)
{
	if(!levels)
		return E_POINTER;

	return m_Graphics.UpdateLevels((LPTIMEDLEVEL)levels);
}

STDMETHODIMP CAnalyzed::GetColorBackground(LONG* color)
{
	if(!color)
		return E_POINTER;

	*color = m_Graphics.GetColorBackground();

	return S_OK;
}

STDMETHODIMP CAnalyzed::SetColorBackground(LONG color)
{
	m_Graphics.SetColorBackground(UNPACK_COLOR((ULONG)color));

	return S_OK;
}

STDMETHODIMP CAnalyzed::GetColorTop(LONG* color)
{
	if(!color)
		return E_POINTER;

	*color = m_Graphics.GetColorTop();

	return S_OK;
}

STDMETHODIMP CAnalyzed::SetColorTop(LONG color)
{
	m_Graphics.SetColorTop(UNPACK_COLOR((ULONG)color));

	return S_OK;
}

STDMETHODIMP CAnalyzed::GetColorTopEnd(LONG* color)
{
	if(!color)
		return E_POINTER;

	*color = m_Graphics.GetColorTopEnd();

	return S_OK;
}

STDMETHODIMP CAnalyzed::SetColorTopEnd(LONG color)
{
	m_Graphics.SetColorTopEnd(UNPACK_COLOR((ULONG)color));

	return S_OK;
}

STDMETHODIMP CAnalyzed::GetColorBottom(LONG* color)
{
	if(!color)
		return E_POINTER;

	*color = m_Graphics.GetColorBottom();

	return S_OK;
}

STDMETHODIMP CAnalyzed::SetColorBottom(LONG color)
{
	m_Graphics.SetColorBottom(UNPACK_COLOR((ULONG)color));

	return S_OK;
}

STDMETHODIMP CAnalyzed::GetColorBottomEnd(LONG* color)
{
	if(!color)
		return E_POINTER;

	*color = m_Graphics.GetColorBottomEnd();

	return S_OK;
}

STDMETHODIMP CAnalyzed::SetColorBottomEnd(LONG color)
{
	m_Graphics.SetColorBottomEnd(UNPACK_COLOR((ULONG)color));

	return S_OK;
}

STDMETHODIMP CAnalyzed::GetColorPeak(LONG* color)
{
	if(!color)
		return E_POINTER;

	*color = m_Graphics.GetColorPeak();

	return S_OK;
}

STDMETHODIMP CAnalyzed::SetColorPeak(LONG color)
{
	m_Graphics.SetColorPeak(UNPACK_COLOR((ULONG)color));

	return S_OK;
}