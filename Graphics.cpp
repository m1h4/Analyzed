#include "Globals.h"
#include "Graphics.h"
#include "Resource.h"
#include "Visualizations.h"

#pragma warning(disable : 4244)	// Conversions (double to float etc.)

CGraphics::CGraphics(void) :
m_TimerFrameRate(60),	// The relative framerate
m_PeakTimeout(15.0f),
m_PeakGravity(-0.2f),
m_Speed(0.3f),
m_TimerElapsed(NULL),
m_TimerAbsolute(NULL),
m_TimerFrequency(NULL),
m_TimerLast(NULL),
m_Renderer(NULL),
m_RendererId(NULL),
m_Visualization(NULL),
m_Hwnd(NULL),
m_Direct3D(NULL),
m_Device(NULL),
m_Direct3DLibrary(NULL),
m_Direct3DCreate9(NULL),
m_Direct3DSetRegion(NULL),
m_Direct3DBeginEvent(NULL),
m_Direct3DEndEvent(NULL),
m_SpriteBuffer(NULL),
m_LineBuffer(NULL),
m_SpriteTexture(NULL),
m_SpriteData(NULL),
m_LineData(NULL),
m_SpriteCount(NULL),
m_LineCount(NULL),
m_NeedReset(FALSE),
m_Blur(FALSE),
m_BlurTexture(NULL),
m_VisualizationTexture(NULL),
m_Background(DEFAULT_COLOR_BACKGROUND),
m_Top(DEFAULT_COLOR_TOP),
m_TopEnd(DEFAULT_COLOR_TOPEND),
m_Bottom(DEFAULT_COLOR_BOTTOM),
m_BottomEnd(DEFAULT_COLOR_BOTTOMEND),
m_Peak(DEFAULT_COLOR_PEAK),
m_LevelsBuffer(NULL),
m_Levels(NULL),
m_Waveform(NULL),
m_WaveformBuffer(NULL)
{
	srand(GetTickCount());
}

CGraphics::~CGraphics(void)
{
	UninitializeRenderer();
	UninitializeDisplay();
	UninitializeTimer();
	Uninitialize();
}

HRESULT CGraphics::Initialize(void)
{
	InitializeCriticalSection(&m_UpdateSection);

	m_Direct3DLibrary = LoadLibrary(D3DDLL);
	if(!m_Direct3DLibrary)
	{
		TRACE(TEXT("Error: Failed to load \"%s\".\n"),D3DDLL);
		return E_FAIL;
	}

	m_Direct3DCreate9 = (LPDIRECT3DCREATE9)GetProcAddress(m_Direct3DLibrary,"Direct3DCreate9");
	if(!m_Direct3DCreate9)
	{
		Uninitialize();
		TRACE(TEXT("Error: Failed to find \"Direct3DCreate9\" in \"%s\".\n"),D3DDLL);
		return NULL;
	}

	m_Direct3DSetRegion = (LPDIRECT3DSETREGION)GetProcAddress(m_Direct3DLibrary,"D3DPERF_SetRegion");
	if(!m_Direct3DSetRegion)
	{
		Uninitialize();
		TRACE(TEXT("Error: Failed to find \"D3DPERF_SetRegion\" in \"%s\".\n"),D3DDLL);
		return NULL;
	}

	m_Direct3DBeginEvent = (LPDIRECT3DBEGINEVENT)GetProcAddress(m_Direct3DLibrary,"D3DPERF_BeginEvent");
	if(!m_Direct3DBeginEvent)
	{
		Uninitialize();
		TRACE(TEXT("Error: Failed to find \"D3DPERF_BeginEvent\" in \"%s\".\n"),D3DDLL);
		return NULL;
	}

	m_Direct3DEndEvent = (LPDIRECT3DENDEVENT)GetProcAddress(m_Direct3DLibrary,"D3DPERF_EndEvent");
	if(!m_Direct3DEndEvent)
	{
		Uninitialize();
		TRACE(TEXT("Error: Failed to find \"D3DPERF_EndEvent\" in \"%s\".\n"),D3DDLL);
		return NULL;
	}

	return S_OK;
}

HRESULT CGraphics::Uninitialize(void)
{
	DeleteCriticalSection(&m_UpdateSection);

	FreeLibrary(m_Direct3DLibrary);
	m_Direct3DLibrary = NULL;

	m_Direct3DCreate9 = NULL;
	m_Direct3DSetRegion = NULL;
	m_Direct3DBeginEvent = NULL;
	m_Direct3DEndEvent = NULL;

	for(UINT i = 0; i < m_Visualizations.GetSize(); ++i)
		delete m_Visualizations[i];

	if(m_Visualizations.GetSize())
		m_Visualizations.Erase();

	return S_OK;
}

HRESULT CGraphics::InitializeRenderer(void)
{
	// Check if renderer already initialized
	if(m_Renderer)
		return S_FALSE;

	m_Renderer = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)RendererThread,this,NULL,&m_RendererId);
	if(!m_Renderer)
	{
		TRACE(TEXT("Failed to create renderer thread %#X.\n"),GetLastError());
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGraphics::UninitializeRenderer(void)
{
	// Check if already uninitialized
	if(!m_Renderer)
		return S_FALSE;

	PostThreadMessage(m_RendererId,WM_QUIT,NULL,NULL);
	
	if(WaitForSingleObject(m_Renderer,2000) == WAIT_TIMEOUT)
		TerminateThread(m_Renderer,0x000000FF);

	CloseHandle(m_Renderer);
	m_Renderer = NULL;
	m_RendererId = NULL;

	return S_OK;
}

HRESULT CGraphics::InitializeTimer(void)
{
	// Get the system timer frequency
	if(!QueryPerformanceFrequency((PLARGE_INTEGER)&m_TimerFrequency))
		return E_FAIL;

	// Get the current time
	if(!QueryPerformanceCounter((PLARGE_INTEGER)&m_TimerLast))
		return E_FAIL;

	// Set the inital elapsed and absolute times to zero
	m_TimerElapsed = NULL;
	m_TimerAbsolute = NULL;

	return S_OK;
}

HRESULT CGraphics::UninitializeTimer(void)
{
	m_TimerElapsed = NULL;
	m_TimerAbsolute = NULL;
	m_TimerFrequency = NULL;
	m_TimerLast = NULL;

	return S_OK;
}

HRESULT CGraphics::InitializeDisplay(HWND hWnd,UINT width,UINT height,BOOL blur)
{
	// Check if windowed visualization (skin mode not supported)
	if(!hWnd)
		return E_FAIL;

	// Safe to assume that if device is not null display is initialized
	if(m_Device)
		UninitializeDisplay();

	// Get the address of the create function
	if(!m_Direct3DCreate9)
	{
		TRACE(TEXT("Error: Failed to find \"Direct3DCreate9\" in \"%s\".\n"),D3DDLL);
		return E_FAIL;
	}

	// Reset audio data
	if(IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
	{
		TRACE(TEXT("Info: Using SSE instruction set.\n"));

		m_Levels = (PFLOAT)_aligned_malloc(sizeof(FLOAT) * VISUALIZATION_BARCOUNT,16);
		m_LevelsBuffer = (PFLOAT)_aligned_malloc(sizeof(FLOAT) * VISUALIZATION_BARCOUNT,16);
		m_Waveform = (PFLOAT)_aligned_malloc(sizeof(FLOAT) * SA_BUFFER_SIZE,16);
		m_WaveformBuffer = (PFLOAT)_aligned_malloc(sizeof(FLOAT) * SA_BUFFER_SIZE,16);
	}
	else
	{
		m_Levels = (PFLOAT)malloc(sizeof(FLOAT) * VISUALIZATION_BARCOUNT);
		m_LevelsBuffer = (PFLOAT)malloc(sizeof(FLOAT) * VISUALIZATION_BARCOUNT);
		m_Waveform = (PFLOAT)malloc(sizeof(FLOAT) * SA_BUFFER_SIZE);
		m_WaveformBuffer = (PFLOAT)malloc(sizeof(FLOAT) * SA_BUFFER_SIZE);
	}

	ZeroMemory(m_Levels,sizeof(FLOAT) * VISUALIZATION_BARCOUNT);
	ZeroMemory(m_LevelsBuffer,sizeof(FLOAT) * VISUALIZATION_BARCOUNT);
	ZeroMemory(m_Waveform,sizeof(FLOAT) * SA_BUFFER_SIZE);
	ZeroMemory(m_WaveformBuffer,sizeof(FLOAT) * SA_BUFFER_SIZE);
	ZeroMemory(m_Peaks,sizeof(m_Peaks));

	m_Hwnd = hWnd;
	m_Blur = blur;

	m_Direct3D = m_Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_Direct3D)
	{
		TRACE(TEXT("Error: Failed to create direct 3d.\n"));
		return E_FAIL;
	}

	m_Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&m_Caps);
	m_Direct3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT,NULL,&m_AdapterIdentifier);

	ZeroMemory(&m_PresentParameters,sizeof(m_PresentParameters));
	m_PresentParameters.Windowed					= TRUE;
	m_PresentParameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_PresentParameters.BackBufferFormat			= D3DFMT_X8R8G8B8;
    //m_PresentParameters.EnableAutoDepthStencil		= TRUE;
	m_PresentParameters.AutoDepthStencilFormat		= D3DFMT_D16;
	m_PresentParameters.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;
	//m_PresentParameters.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_PresentParameters.BackBufferWidth				= width;
	m_PresentParameters.BackBufferHeight			= height;

	//m_PresentParameters.MultiSampleType				= D3DMULTISAMPLE_4_SAMPLES;

	DWORD vp = NULL;

	if(m_Caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
	{
		vp |= D3DCREATE_PUREDEVICE;
		TRACE(TEXT("Info: Using pure device.\n"));
	}
	
	if(m_Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		TRACE(TEXT("Info: Using hardware vertex processing.\n"));
	}
	else
	{
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		TRACE(TEXT("Info: Using software vertex processing.\n"));
	}

	if(FAILED(m_Direct3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_Hwnd,vp|D3DCREATE_MULTITHREADED,&m_PresentParameters,&m_Device)))
	{
		TRACE(TEXT("Error: Failed to create direct 3d device.\n"));
		return E_FAIL;
	}

	if(FAILED(Restore()))
	{
		TRACE(TEXT("Error: Failed to initaly restore device.\n"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGraphics::UninitializeDisplay(void)
{
	Invalidate();

	SAFERELEASE(m_Device);
	SAFERELEASE(m_Direct3D);

	if(IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
	{
		_aligned_free(m_Levels);
		_aligned_free(m_LevelsBuffer);
		_aligned_free(m_Waveform);
		_aligned_free(m_WaveformBuffer);
	}
	else
	{
		free(m_Levels);
		free(m_LevelsBuffer);
		free(m_Waveform);
		free(m_WaveformBuffer);
	}

	m_Levels = NULL;
	m_LevelsBuffer = NULL;
	m_Waveform = NULL;
	m_WaveformBuffer = NULL;

	m_Hwnd = NULL;

	return S_OK;
}

HRESULT CGraphics::Render(void)
{
	if(!m_Device)
		return E_FAIL;

	if(m_NeedReset)
		return S_FALSE;

	HRESULT hr;
	if(FAILED(hr = m_Device->TestCooperativeLevel()))
	{
		TRACE(TEXT("Warning: Failed on test cooperative level.\n"));

		// The device has been lost but cannot be reset at this time. 
		// Therefore, rendering is not possible and we'll have to return 
		// and try again at a later time.
		if(hr == D3DERR_DEVICELOST)
		{
			TRACE(TEXT("Warning: Display device lost.\n"));
			return S_FALSE;
		}

		// The device has been lost but it can be reset at this time. 
		if(hr == D3DERR_DEVICENOTRESET)
		{
			// Try to reset the device
			m_NeedReset = TRUE;
			return S_FALSE;
		}
	}
	
	if(SUCCEEDED(m_Device->BeginScene()))
	{
		if(SUCCEEDED(BeginWrite()))
		{
			//m_Direct3DBeginEvent(NULL,TEXT("Rendering"));

			if(!m_Blur)
			{
				m_Device->Clear(NULL,NULL,D3DCLEAR_TARGET,m_Background,1.0f,NULL);

				if(GetVisualizationCount() && GetVisualization(m_Visualization))
					GetVisualization(m_Visualization)->Draw(*this);
			}
			else
			{
				LPDIRECT3DSURFACE9 backbufferTarget = NULL;
				m_Device->GetRenderTarget(0,&backbufferTarget);
				
				LPDIRECT3DSURFACE9 blurTarget = NULL;
				m_BlurTexture->GetSurfaceLevel(0,&blurTarget);
				m_Device->SetRenderTarget(0,blurTarget);

				m_Device->Clear(NULL,NULL,D3DCLEAR_TARGET,Color(0.0f,0.0f,0.0f,0.0f),1.0f,NULL);
				// Maybe this is faster
				//SpriteWrite(Rect(0.0f,0.0f,GetWidth()/2,GetHeight()/2),Color(1.0f,0.0f,0.0f,0.0f),NULL);

				SpriteWrite(Rect(0.0f,0.0f,GetWidth()/2,GetHeight()/2),Color(1.0f,1.0f,1.0f,1.0f),m_VisualizationTexture);

				SpriteFlush();
				LineFlush();

				LPDIRECT3DSURFACE9 visualizationTarget = NULL;
				m_VisualizationTexture->GetSurfaceLevel(0,&visualizationTarget);
				m_Device->SetRenderTarget(0,visualizationTarget);
				
				SpriteWrite(Rect(0.0f,0.0f,GetWidth(),GetHeight()),Color(1.0f,1.0f,1.0f,1.0f),m_BlurTexture);

				SpriteFlush();

				m_Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
				m_Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
				m_Device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);

				SpriteWrite(Rect(0.0f,0.0f,GetWidth(),GetHeight()),Color(max(0.0f,min(1.0f,GetElapsedTime()/50.0f)),1.0f,1.0f,1.0f),NULL);

				SpriteFlush();

				m_Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
				m_Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
				m_Device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);

				if(GetVisualizationCount() && GetVisualization(m_Visualization))
					GetVisualization(m_Visualization)->Draw(*this);

				SpriteFlush();
				LineFlush();
			
				m_Device->SetRenderTarget(0,backbufferTarget);

				visualizationTarget->Release();
				blurTarget->Release();
				backbufferTarget->Release();

				SpriteWrite(Rect(0.0f,0.0f,GetWidth(),GetHeight()),Color(1.0f,1.0f,1.0f,1.0f),m_VisualizationTexture);
			}

			EndWrite();

			//m_Direct3DEndEvent();
		}
		else
		{
			TRACE(TEXT("Error: Failed to begin sprite write.\n"));
		}

		m_Device->EndScene();
	}
	else
	{
		TRACE(TEXT("Error: Failed to begin scene.\n"));
	}

	if(FAILED(m_Device->Present(NULL,NULL,NULL,NULL)))
	{
		TRACE(TEXT("Error: Failed to present scene.\n"));
	}

	return S_OK;
}

HRESULT CGraphics::SpriteWrite(const PointF& topleft,const PointF& topright,const PointF& bottomright,const PointF& bottomleft,const PointF& ttopleft,const PointF& tbottomright,const Color& color,IDirect3DTexture9* texture)
{
	if(m_LineCount)
		LineFlush();

	if(m_SpriteCount + 6 >= GRAPHICS_SPRITE_CACHE || (m_SpriteCount && m_SpriteTexture != texture))
		if(FAILED(SpriteFlush()))
			return E_FAIL;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = topleft.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = topleft.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = topright.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = topright.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = bottomright.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = bottomright.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = bottomright.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = bottomright.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = bottomleft.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = bottomleft.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = topleft.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = topleft.y - 0.5f;
	++m_SpriteCount;

	m_SpriteTexture = texture;

	return S_OK;
}

HRESULT CGraphics::SpriteWrite(const Rect& rect,const Color& colortl,const Color& colortr,const Color& colorbr,const Color& colorbl,IDirect3DTexture9* texture)
{
	if(m_LineCount)
		LineFlush();

	if(m_SpriteCount + 6 >= GRAPHICS_SPRITE_CACHE || (m_SpriteCount && m_SpriteTexture != texture))
		if(FAILED(SpriteFlush()))
			return E_FAIL;

	m_SpriteData[m_SpriteCount].color = colortl;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = rect.left - 0.5f;
	m_SpriteData[m_SpriteCount].y = rect.top - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colortr;
	m_SpriteData[m_SpriteCount].u = 1.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = rect.right - 0.5f;
	m_SpriteData[m_SpriteCount].y = rect.top - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorbr;
	m_SpriteData[m_SpriteCount].u = 1.0f;
	m_SpriteData[m_SpriteCount].v = 1.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = rect.right - 0.5f;
	m_SpriteData[m_SpriteCount].y = rect.bottom - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorbr;
	m_SpriteData[m_SpriteCount].u = 1.0f;
	m_SpriteData[m_SpriteCount].v = 1.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = rect.right - 0.5f;
	m_SpriteData[m_SpriteCount].y = rect.bottom - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorbl;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 1.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = rect.left - 0.5f;
	m_SpriteData[m_SpriteCount].y = rect.bottom - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colortl;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = rect.left - 0.5f;
	m_SpriteData[m_SpriteCount].y = rect.top - 0.5f;
	++m_SpriteCount;

	m_SpriteTexture = texture;

	return S_OK;
}

HRESULT CGraphics::SpriteWrite(const PointF& center,PointF size,const PointF& ttopleft,const PointF& tbottomright,const Color& colorTop,const Color& colorBottom,IDirect3DTexture9* texture)
{
	if(m_LineCount)
		LineFlush();

	if(m_SpriteCount + 6 >= GRAPHICS_SPRITE_CACHE || (m_SpriteCount && m_SpriteTexture != texture))
		if(FAILED(SpriteFlush()))
			return E_FAIL;

	m_SpriteData[m_SpriteCount].color = colorTop;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - size.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - size.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorTop;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x + size.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - size.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorBottom;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x + size.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y + size.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorBottom;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x + size.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y + size.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorBottom;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - size.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y + size.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = colorTop;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - size.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - size.y - 0.5f;
	++m_SpriteCount;

	m_SpriteTexture = texture;

	return S_OK;
}

HRESULT CGraphics::SpriteWrite(const PointF& center,const PointF* vertices,unsigned long verticesCount,const Color& color)
{
	if(m_LineCount)
		LineFlush();

	if(m_SpriteCount + verticesCount * 3 + 3 >= GRAPHICS_SPRITE_CACHE || (m_SpriteCount && m_SpriteTexture != NULL))
		if(FAILED(SpriteFlush()))
			return E_FAIL;

	unsigned long i;

	for(i = 1; i < verticesCount; ++i)
	{
		m_SpriteData[m_SpriteCount].color = color;
		m_SpriteData[m_SpriteCount].u = 0.0f;
		m_SpriteData[m_SpriteCount].v = 0.0f;
		m_SpriteData[m_SpriteCount].z = 0.5f;
		m_SpriteData[m_SpriteCount].h = 1.0f;
		m_SpriteData[m_SpriteCount].x = center.x - 0.5f;
		m_SpriteData[m_SpriteCount].y = center.y - 0.5f;
		++m_SpriteCount;

		m_SpriteData[m_SpriteCount].color = color;
		m_SpriteData[m_SpriteCount].u = 0.0f;
		m_SpriteData[m_SpriteCount].v = 0.0f;
		m_SpriteData[m_SpriteCount].z = 0.5f;
		m_SpriteData[m_SpriteCount].h = 1.0f;
		m_SpriteData[m_SpriteCount].x = vertices[i-1].x + center.x - 0.5f;
		m_SpriteData[m_SpriteCount].y = vertices[i-1].y + center.y - 0.5f;
		++m_SpriteCount;

		m_SpriteData[m_SpriteCount].color = color;
		m_SpriteData[m_SpriteCount].u = 0.0f;
		m_SpriteData[m_SpriteCount].v = 0.0f;
		m_SpriteData[m_SpriteCount].z = 0.5f;
		m_SpriteData[m_SpriteCount].h = 1.0f;
		m_SpriteData[m_SpriteCount].x = vertices[i].x + center.x - 0.5f;
		m_SpriteData[m_SpriteCount].y = vertices[i].y + center.y - 0.5f;
		++m_SpriteCount;
	}
	
	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = vertices[i-1].x + center.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = vertices[i-1].y + center.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = vertices[0].x + center.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = vertices[0].y + center.y - 0.5f;
	++m_SpriteCount;

	m_SpriteTexture = NULL;

	return S_OK;
}

HRESULT CGraphics::SpriteWrite(const PointF& center,const PointF* vertices,unsigned long verticesCount,float angle,const Color& color)
{
	if(m_LineCount)
		LineFlush();

	if(m_SpriteCount + verticesCount * 3 + 3 >= GRAPHICS_SPRITE_CACHE || (m_SpriteCount && m_SpriteTexture != NULL))
		if(FAILED(SpriteFlush()))
			return E_FAIL;

	float cangle = cos(angle);
	float sangle = sin(angle);

	unsigned long i;

	for(i = 1; i < verticesCount; ++i)
	{
		m_SpriteData[m_SpriteCount].color = color;
		m_SpriteData[m_SpriteCount].u = 0.0f;
		m_SpriteData[m_SpriteCount].v = 0.0f;
		m_SpriteData[m_SpriteCount].z = 0.5f;
		m_SpriteData[m_SpriteCount].h = 1.0f;
		m_SpriteData[m_SpriteCount].x = center.x - 0.5f;
		m_SpriteData[m_SpriteCount].y = center.y - 0.5f;
		++m_SpriteCount;

		m_SpriteData[m_SpriteCount].color = color;
		m_SpriteData[m_SpriteCount].u = 0.0f;
		m_SpriteData[m_SpriteCount].v = 0.0f;
		m_SpriteData[m_SpriteCount].z = 0.5f;
		m_SpriteData[m_SpriteCount].h = 1.0f;
		m_SpriteData[m_SpriteCount].x = vertices[i-1].x * cangle - vertices[i-1].y * sangle + center.x - 0.5f;
		m_SpriteData[m_SpriteCount].y = vertices[i-1].y * cangle + vertices[i-1].x * sangle + center.y - 0.5f;
		++m_SpriteCount;

		m_SpriteData[m_SpriteCount].color = color;
		m_SpriteData[m_SpriteCount].u = 0.0f;
		m_SpriteData[m_SpriteCount].v = 0.0f;
		m_SpriteData[m_SpriteCount].z = 0.5f;
		m_SpriteData[m_SpriteCount].h = 1.0f;
		m_SpriteData[m_SpriteCount].x = vertices[i].x * cangle - vertices[i].y * sangle + center.x - 0.5f;
		m_SpriteData[m_SpriteCount].y = vertices[i].y * cangle + vertices[i].x * sangle + center.y - 0.5f;
		++m_SpriteCount;
	}
	
	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = vertices[i-1].x * cangle - vertices[i-1].y * sangle + center.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = vertices[i-1].y * cangle + vertices[i-1].x * sangle + center.y - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = 0.0f;
	m_SpriteData[m_SpriteCount].v = 0.0f;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = vertices[0].x * cangle - vertices[0].y * sangle + center.x - 0.5f;
	m_SpriteData[m_SpriteCount].y = vertices[0].y * cangle + vertices[0].x * sangle + center.y - 0.5f;
	++m_SpriteCount;

	m_SpriteTexture = NULL;

	return S_OK;
}

HRESULT CGraphics::SpriteWrite(const PointF& center,PointF size,const PointF& ttopleft,const PointF& tbottomright,float angle,const Color& color,IDirect3DTexture9* texture)
{
	if(m_LineCount)
		LineFlush();

	if(m_SpriteCount + 6 >= GRAPHICS_SPRITE_CACHE || (m_SpriteCount && m_SpriteTexture != texture))
		if(FAILED(SpriteFlush()))
			return E_FAIL;

	float cangle = cos(angle);
	float sangle = sin(angle);

	size.x /= 2.0f;
	size.y /= 2.0f;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - (size.x * cangle - size.y * sangle) - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - (size.y * cangle + size.x * sangle) - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x + (size.x * cangle - size.y * sangle) - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - (size.y * cangle + size.x * sangle) - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x + (size.x * cangle - size.y * sangle) - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y + (size.y * cangle + size.x * sangle) - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = tbottomright.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x + (size.x * cangle - size.y * sangle) - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y + (size.y * cangle + size.x * sangle) - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = tbottomright.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - (size.x * cangle - size.y * sangle) - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y + (size.y * cangle + size.x * sangle) - 0.5f;
	++m_SpriteCount;

	m_SpriteData[m_SpriteCount].color = color;
	m_SpriteData[m_SpriteCount].u = ttopleft.x;
	m_SpriteData[m_SpriteCount].v = ttopleft.y;
	m_SpriteData[m_SpriteCount].z = 0.5f;
	m_SpriteData[m_SpriteCount].h = 1.0f;
	m_SpriteData[m_SpriteCount].x = center.x - (size.x * cangle - size.y * sangle)  - 0.5f;
	m_SpriteData[m_SpriteCount].y = center.y - (size.y * cangle + size.x * sangle) - 0.5f;
	++m_SpriteCount;

	m_SpriteTexture = texture;

	return S_OK;
}

HRESULT CGraphics::LineWrite(const PointF& start,const PointF& end,const Color& color)
{
	if(m_SpriteCount)
		SpriteFlush();

	if(m_LineCount + 2 >= GRAPHICS_LINE_CACHE)
		if(!LineFlush())
			return false;

	//if(m_LineCount)	????
	//	LineFlush();	????

	m_LineData[m_LineCount].color = color;
	m_LineData[m_LineCount].z = 0.5f;
	m_LineData[m_LineCount].h = 1.0f;
	m_LineData[m_LineCount].x = start.x - 0.5f;
	m_LineData[m_LineCount].y = start.y - 0.5f;
	++m_LineCount;

	m_LineData[m_LineCount].color = color;
	m_LineData[m_LineCount].z = 0.5f;
	m_LineData[m_LineCount].h = 1.0f;
	m_LineData[m_LineCount].x = end.x - 0.5f;
	m_LineData[m_LineCount].y = end.y - 0.5f;
	++m_LineCount;

	return true;
}

UINT Step(UINT bar,UINT bars)
{
	float stepf = 10.0f;
	float count = stepf * ((float)bars*((float)bars+1)) / 2;

	return ((stepf * ((float)bar*((float)bar+1)) / 2)/count)*SA_BUFFER_SIZE;
}

HRESULT CGraphics::UpdateLevels(LPTIMEDLEVEL levels)
{
	AUTO_SECTION(m_UpdateSection);

	// Here we use the calling thread's context to do a device reset
	if(m_NeedReset)
		Reset();

	if(!levels)
		return E_POINTER;

	// Don't update if paused
	if(levels->state == pause_state)
		return S_OK;

	// Just average the channels and cast to fp
	for(UINT i = 0; i < VISUALIZATION_BARCOUNT; ++i)
	{
		m_LevelsBuffer[i] = 0.0f;

		for(UINT x = i * SA_BUFFER_SIZE/VISUALIZATION_BARCOUNT; x < (i + 1) * SA_BUFFER_SIZE/VISUALIZATION_BARCOUNT; ++x)
			m_LevelsBuffer[i] += (levels->frequency[0][x] + levels->frequency[1][x]) / 2.0f;

		m_LevelsBuffer[i] /= SA_BUFFER_SIZE/VISUALIZATION_BARCOUNT;

		//for(UINT x = Step(i,VISUALIZATION_BARCOUNT+1); x < Step(i+1,VISUALIZATION_BARCOUNT+1); ++x)
		//	m_LevelsBuffer[i] += (levels->frequency[0][x] + levels->frequency[1][x]) / 2.0f;

		//m_LevelsBuffer[i] /= Step(i+1,VISUALIZATION_BARCOUNT+1) - Step(i,VISUALIZATION_BARCOUNT+1);

		m_LevelsBuffer[i] *= 2.0f;
	}

	for(UINT i = 0; i < SA_BUFFER_SIZE; ++i)
		m_WaveformBuffer[i] = (levels->waveform[0][i] + levels->waveform[1][i]) / 2.0f - 128.0f;

	return S_OK;
}

DOUBLE CGraphics::GetMagnitude(double level,double scale,double magnitude,double cutrange,double sharpness,unsigned long x,unsigned long size)
{
	double powerbase = log10(1.0 + cutrange) / pow((double)size - 1.0,sharpness);
	double power = pow(10.0,powerbase);

	// Compute how much to clip this band and clip it
	return scale * (log10(level / pow(10.0,magnitude)) - pow(power,pow((double)size - 1.0 - (double)x,sharpness)) + 1.0);
}

HRESULT CGraphics::UpdateTimer(void)
{
	LONGLONG time;

	if(!QueryPerformanceCounter((PLARGE_INTEGER)&time))
		return E_FAIL;

	// Calculate the elapsed time
	m_TimerElapsed = DOUBLE(time - m_TimerLast) / DOUBLE(m_TimerFrequency) * DOUBLE(m_TimerFrameRate);
	m_TimerAbsolute += m_TimerElapsed;
	m_TimerLast = time;

	return S_OK;
}

HRESULT CGraphics::Update(void)
{
	AUTO_SECTION(m_UpdateSection);

	FLOAT factor = min(GetElapsedTime() * m_Speed,1.0f);

	if(IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
	{
		// Update the levels
		for(UINT i = 0; i < VISUALIZATION_BARCOUNT; i += 4)
		{
			//SSELerpArr(m_Levels + i,m_Levels + i,m_LevelsBuffer + i,factor);

			_mm_store_ps(m_Levels + i,_mm_add_ps(_mm_mul_ps(_mm_sub_ps(_mm_load_ps(m_LevelsBuffer + i),_mm_load_ps(m_Levels + i)),_mm_load_ps1(&factor)),_mm_load_ps(m_Levels + i)));
		}

		// Update the waveform
		for(UINT i = 0; i < SA_BUFFER_SIZE; i += 4)
		{
			//SSELerpArr(m_Waveform + i,m_Waveform + i,m_WaveformBuffer + i,factor);

			_mm_store_ps(m_Waveform + i,_mm_add_ps(_mm_mul_ps(_mm_sub_ps(_mm_load_ps(m_WaveformBuffer + i),_mm_load_ps(m_Waveform + i)),_mm_load_ps1(&factor)),_mm_load_ps(m_Waveform + i)));
		}
	}
	else
	{
		// Update the levels
		for(UINT i = 0; i < VISUALIZATION_BARCOUNT; ++i)
			m_Levels[i] = m_Levels[i] + min(GetElapsedTime() * m_Speed,1.0f) * (m_LevelsBuffer[i] - m_Levels[i]);

		// Update the waveform
		for(UINT i = 0; i < SA_BUFFER_SIZE; ++i)
			m_Waveform[i] = m_Waveform[i] + min(GetElapsedTime() * m_Speed,1.0f) * (m_WaveformBuffer[i] - m_Waveform[i]);
	}
	
	// Go through all the peaks and update each
	for(UINT i = 0; i < VISUALIZATION_BARCOUNT; ++i)
	{
		// Update the position and velocity
		if(m_Peaks[i].timeout <= 0.0f)
		{
			m_Peaks[i].position += m_Peaks[i].velocity * min(GetElapsedTime(),1.0f);
			m_Peaks[i].velocity += m_PeakGravity * min(GetElapsedTime(),1.0f);
		}
		else
			m_Peaks[i].timeout -= min(GetElapsedTime(),1.0f);

		// Check if it has collided with a bar
		if(m_Peaks[i].position < m_Levels[i])
		{
			m_Peaks[i].position = m_Levels[i];
			m_Peaks[i].velocity = 0.0f;
			m_Peaks[i].timeout = m_PeakTimeout;
		}
	}
	
	return S_OK;
}

HRESULT CGraphics::SetVisualization(LONG visualization)
{
	// Check for overflow
	if(visualization < 0 || visualization > (LONG)GetVisualizationCount() - 1)
		return E_INVALIDARG;

	m_Visualization = visualization;

	//for(UINT i = 0; i < VISUALIZATION_BARCOUNT; ++i)
	//{
	//	TRACE("%d: %d-%d (%d)\n",i,Step(i,VISUALIZATION_BARCOUNT+1),Step(i+1,VISUALIZATION_BARCOUNT+1),Step(i+1,VISUALIZATION_BARCOUNT+1)-Step(i,VISUALIZATION_BARCOUNT+1));
	//}

	return S_OK;
}

BOOL CGraphics::GetWindowVisible(void)
{
	// Check if initialized
	if(!m_Hwnd)
		return FALSE;

	RECT rect;
	GetClientRect(m_Hwnd,&rect);

	return IsWindowVisible(m_Hwnd) && !IsIconic(m_Hwnd) && !IsRectEmpty(&rect) && IsWindowEnabled(m_Hwnd);
}

ULONG WINAPI RendererThread(CGraphics* graphics)
{
	// This should be done automatically but it somehow isn't
	SETTHREADNAME(__FUNCTION__);

	MSG msg;
	ZeroMemory(&msg,sizeof(msg));

	SetTimer(NULL,NULL,15,NULL);

	while(GetMessage(&msg,NULL,NULL,NULL) > 0)
	{
		if(msg.message == WM_QUIT)
			break;

		if(msg.message != WM_TIMER)
			TRACE(TEXT("Warning: Got non-timer message: %d\n"),msg.message);

		graphics->UpdateTimer();
		graphics->Update();

		// Don't draw if window not visible
		if(graphics->GetWindowVisible())
			graphics->Render();
		else
			Sleep(50);
	}

	KillTimer(NULL,NULL);

	return 0x0000000;
}

HRESULT CGraphics::Reset(void)
{
	m_NeedReset = FALSE;

	// If the device can be restored, the application prepares the 
	// device by destroying all video-memory resources and any 
	// swap chains
	if(FAILED(Invalidate()))
	{
		TRACE(TEXT("Error: Failed to invalidate device.\n"));
		return E_FAIL;
	}

	// Try to reset the device
	HRESULT hr = m_Device->Reset(&m_PresentParameters);
	if(FAILED(hr))
	{
		// Check if error other than device lost
		if(hr != D3DERR_DEVICELOST)
		{
			HWND hWnd = GetWindowHandle();
			UINT width = GetWidth();
			UINT height = GetHeight();
			BOOL blur = GetBlur();

			UninitializeRenderer();
			UninitializeDisplay();
			InitializeDisplay(hWnd,width,height,blur);
			InitializeRenderer();
			TRACE(TEXT("Warning: Failed to reset the display device, reinitialized graphics.\n"));
			return S_FALSE;
		}

		// The device was lost again, so continue waiting until it can be reset
		TRACE(TEXT("Error: Failed to reset the display device.\n"));
		return S_FALSE;
	}

	// Finally, a lost device must re-create resources (including  
	// video memory resources) after it has been reset
	if(FAILED(Restore()))
	{
		HWND hWnd = GetWindowHandle();
		UINT width = GetWidth();
		UINT height = GetHeight();
		BOOL blur = GetBlur();

		UninitializeRenderer();
		UninitializeDisplay();
		InitializeDisplay(hWnd,width,height,blur);
		InitializeRenderer();
		TRACE(TEXT("Warning: Failed to restore device, reinitialized graphics.\n"));
		return false;
	}

	return S_OK;
}

HRESULT CGraphics::Invalidate(void)
{
	SAFERELEASE(m_SpriteBuffer);
	SAFERELEASE(m_LineBuffer);
	SAFERELEASE(m_BlurTexture);
	SAFERELEASE(m_VisualizationTexture);

	return S_OK;
}

HRESULT CGraphics::Restore(void)
{
	if(FAILED(m_Device->CreateVertexBuffer(sizeof(SPRITEVERTEX) * GRAPHICS_SPRITE_CACHE,D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,SPRITEVERTEXFORMAT,D3DPOOL_DEFAULT,&m_SpriteBuffer,NULL)))
	{
		TRACE(TEXT("Error: Failed to create sprite vertex buffer (%u bytes).\n"),sizeof(SPRITEVERTEX) * GRAPHICS_SPRITE_CACHE);
		return E_FAIL;
	}

	if(FAILED(m_Device->CreateVertexBuffer(sizeof(LINEVERTEX) * GRAPHICS_LINE_CACHE,D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,LINEVERTEXFORMAT,D3DPOOL_DEFAULT,&m_LineBuffer,NULL)))
	{
		TRACE(TEXT("Error: Failed to create line vertex buffer (%u bytes).\n"),sizeof(LINEVERTEX) * GRAPHICS_LINE_CACHE);
		return E_FAIL;
	}

	if(m_Blur)
	{
		if(FAILED(m_Device->CreateTexture(m_PresentParameters.BackBufferWidth,m_PresentParameters.BackBufferHeight,1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&m_VisualizationTexture,NULL)))
		{
			TRACE(TEXT("Error: Failed to create main render target.\n"));
			return E_FAIL;
		}

		if(FAILED(m_Device->CreateTexture(m_PresentParameters.BackBufferWidth / 2,m_PresentParameters.BackBufferHeight / 2,1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&m_BlurTexture,NULL)))
		{
			TRACE(TEXT("Error: Failed to create blur render target.\n"));
			return E_FAIL;
		}
		

		LPDIRECT3DSURFACE9 visualizationSurface = NULL;
		m_VisualizationTexture->GetSurfaceLevel(0,&visualizationSurface);
		m_Device->ColorFill(visualizationSurface,NULL,Color(0.0f,0.0f,0.0f,0.0f));
		visualizationSurface->Release();
	}

	// Set-up for 2D drawing
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	m_Device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
    m_Device->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);

    m_Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG2);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

    m_Device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);

    m_Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

    m_Device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

    m_Device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

	m_Device->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_Device->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);

	m_Device->SetRenderState(D3DRS_ZENABLE,FALSE);

	return S_OK;
}

HRESULT CGraphics::BeginWrite(void)
{
	ASSERT(m_Device);

	if(FAILED(m_SpriteBuffer->Lock(NULL,sizeof(SPRITEVERTEX) * GRAPHICS_SPRITE_CACHE,(LPVOID*)&m_SpriteData,D3DLOCK_DISCARD)))
	{
		TRACE(TEXT("Error: Failed to lock sprite vertex buffer size %d bytes.\n"),sizeof(SPRITEVERTEX) * GRAPHICS_SPRITE_CACHE);
		return E_FAIL;
	}

	if(FAILED(m_LineBuffer->Lock(NULL,sizeof(LINEVERTEX) * GRAPHICS_LINE_CACHE,(LPVOID*)&m_LineData,D3DLOCK_DISCARD)))
	{
		TRACE(TEXT("Error: Failed to lock line vertex buffer size %d bytes.\n"),sizeof(LINEVERTEX) * GRAPHICS_LINE_CACHE);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGraphics::EndWrite(void)
{
	if(FAILED(SpriteFlush()))
		TRACE(TEXT("Warning: Failed to flush sprite buffer.\n"));

	m_SpriteBuffer->Unlock();

	if(FAILED(LineFlush()))
		TRACE(TEXT("Warning: Failed to flush line buffer.\n"));

	m_LineBuffer->Unlock();

	return S_OK;
}

HRESULT CGraphics::SpriteFlush(void)
{
	ASSERT(m_SpriteBuffer);

	if(!m_SpriteCount)
		return S_FALSE;

	m_SpriteBuffer->Unlock();
	m_SpriteData = NULL;

	m_Device->SetFVF(SPRITEVERTEXFORMAT);
	m_Device->SetTexture(0,m_SpriteTexture);
	m_Device->SetStreamSource(0,m_SpriteBuffer,0,sizeof(SPRITEVERTEX));
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,m_SpriteCount / 3);

	m_SpriteCount = NULL;
	m_SpriteTexture = NULL;

	if(FAILED(m_SpriteBuffer->Lock(NULL,sizeof(SPRITEVERTEX) * GRAPHICS_SPRITE_CACHE,(LPVOID*)&m_SpriteData,D3DLOCK_DISCARD)))
	{
		TRACE(TEXT("Error: Failed to lock sprite vertex buffer size %d bytes.\n"),sizeof(SPRITEVERTEX) * GRAPHICS_SPRITE_CACHE);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGraphics::LineFlush(void)
{
	ASSERT(m_LineBuffer);

	if(!m_LineCount)
		return S_FALSE;

	m_LineBuffer->Unlock();
	m_LineData = NULL;

	m_Device->SetFVF(LINEVERTEXFORMAT);
	m_Device->SetTexture(0,NULL);
	m_Device->SetStreamSource(0,m_LineBuffer,0,sizeof(LINEVERTEX));
	m_Device->DrawPrimitive(D3DPT_LINELIST,0,m_LineCount);

	m_LineCount = NULL;

	if(FAILED(m_LineBuffer->Lock(NULL,sizeof(LINEVERTEX) * GRAPHICS_LINE_CACHE,(LPVOID*)&m_LineData,D3DLOCK_DISCARD)))
	{
		TRACE(TEXT("Error: Failed to lock line vertex buffer size %d bytes.\n"),sizeof(LINEVERTEX) * GRAPHICS_LINE_CACHE);
		return E_FAIL;
	}

	return S_OK;
}