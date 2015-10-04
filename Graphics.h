#pragma once

#include "Effects.h"
#include "Resource.h"
#include "Timer.h"
#include "Visualization.h"

#define VISUALIZATION_BARCOUNT 32
#define GRAPHICS_SPRITE_CACHE 32768
#define GRAPHICS_LINE_CACHE 4096

#define DEFAULT_COLOR_TOP Color(0.2f,0.3f,0.7f)
#define DEFAULT_COLOR_TOPEND Color(0.3f,0.4f,0.9f)
#define DEFAULT_COLOR_BOTTOM Color(0.1f,0.2f,0.4f)
#define DEFAULT_COLOR_BOTTOMEND Color(0.2f,0.3f,0.7f)
#define DEFAULT_COLOR_BACKGROUND Color(0.0f,0.0f,0.0f)
#define DEFAULT_COLOR_PEAK Color(0.4f,0.5f,0.9f)

typedef struct
{
	FLOAT x, y, z, h;
	DWORD color;
	FLOAT u, v;
} SPRITEVERTEX,*LPSPRITEVERTEX;

static const DWORD SPRITEVERTEXFORMAT = D3DFVF_XYZRHW|D3DFVF_TEX1|D3DFVF_DIFFUSE;

typedef struct
{
	FLOAT x, y, z, h;
	DWORD color;
} LINEVERTEX,*LPLINEVERTEX;

static const DWORD LINEVERTEXFORMAT = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;

typedef struct
{
	FLOAT	position;
	FLOAT	velocity;
	FLOAT	timeout;
} PEAK,*LPPEAK;

class CAutoCriticalSection
{
public:
	CAutoCriticalSection(CRITICAL_SECTION& section) : m_Section(section) { EnterCriticalSection(&m_Section); }
	~CAutoCriticalSection(VOID) { LeaveCriticalSection(&m_Section); }

protected:
	CRITICAL_SECTION& m_Section;
};

#define AUTO_SECTION(Section) CAutoCriticalSection section(Section);

typedef LPDIRECT3D9 (WINAPI*LPDIRECT3DCREATE9)(UINT);
typedef INT (WINAPI*LPDIRECT3DSETREGION)(D3DCOLOR,LPCWSTR);
typedef INT (WINAPI*LPDIRECT3DBEGINEVENT)(D3DCOLOR,LPCWSTR);
typedef INT (WINAPI*LPDIRECT3DENDEVENT)(VOID);

//#ifdef _DEBUG
//#define D3DDLL TEXT("d3d9d.dll")
//#else
#define D3DDLL TEXT("d3d9.dll")
//#endif

class CGraphics
{
public:
	enum RectFlag
	{
		RectFlagTop = 1,
		RectFlagLeft = 2,
		RectFlagRight = 4,
		RectFlagBottom = 8,
		RectFlagAll = 15,
	};

public:
	CGraphics(VOID);
	~CGraphics(VOID);

	HRESULT Initialize(VOID);
	HRESULT Uninitialize(VOID);

	HRESULT InitializeDisplay(HWND hWnd,UINT width,UINT height,BOOL blur);
	HRESULT UninitializeDisplay(VOID);

	HRESULT InitializeTimer(VOID);
	HRESULT UninitializeTimer(VOID);

	HRESULT InitializeRenderer(VOID);
	HRESULT UninitializeRenderer(VOID);

	HRESULT Render(VOID);

	HRESULT Reset(VOID);
	HRESULT Invalidate(VOID);
	HRESULT Restore(VOID);

	HRESULT BeginWrite(VOID);
	HRESULT EndWrite(VOID);

	HRESULT SpriteWrite(const PointF& topleft,const PointF& topright,const PointF& bottomright,const PointF& bottomleft,const PointF& ttopleft,const PointF& tbottomright,const Color& color,IDirect3DTexture9* texture);
	HRESULT SpriteWrite(const PointF& center,PointF size,const PointF& ttopleft,const PointF& tbottomright,const Color& colorTop,const Color& colorBottom,IDirect3DTexture9* texture);
	HRESULT SpriteWrite(const PointF& center,PointF size,const PointF& ttopleft,const PointF& tbottomright,const Color& color,IDirect3DTexture9* texture) { return SpriteWrite(center,size,ttopleft,tbottomright,color,color,texture); }
	HRESULT SpriteWrite(const PointF& center,PointF size,const PointF& ttopleft,const PointF& tbottomright,float angle,const Color& color,IDirect3DTexture9* texture);
	HRESULT SpriteWrite(const PointF& center,const PointF* vertices,unsigned long verticesCount,const Color& color);
	HRESULT SpriteWrite(const PointF& center,const PointF* vertices,unsigned long verticesCount,float angle,const Color& color);
	HRESULT SpriteWrite(const Rect& rect,const Color& colortl,const Color& colortr,const Color& colorbr,const Color& colorbl,IDirect3DTexture9* texture);
	HRESULT SpriteWrite(const Rect& rect,const Color& color,IDirect3DTexture9* texture) { return SpriteWrite(rect,color,color,color,color,texture); }

	HRESULT SpriteFlush(void);

	HRESULT LineWrite(const PointF& start,const PointF& end,const Color& color);
	HRESULT LineWriteThick(const PointF& start,const PointF& end,const Color& color);

	HRESULT LineFlush(VOID);

	HRESULT Update(VOID);
	HRESULT	UpdateTimer(VOID);
	HRESULT UpdateLevels(LPTIMEDLEVEL levels);

	HRESULT AddVisualization(CVisualization* visualization) { m_Visualizations.InsertBack(visualization); return S_OK; }
	HRESULT SetVisualization(LONG visualization);
	ULONG GetVisualization(VOID) const { return m_Visualization; }
	CVisualization* GetVisualization(UINT index) const { return m_Visualizations[index]; }
	ULONG GetVisualizationCount(VOID) const { return m_Visualizations.GetSize(); }

	DOUBLE GetElapsedTime(VOID) const { return m_TimerElapsed; }
	DOUBLE GetAbsoluteTime(VOID) const { return m_TimerAbsolute; }

	BOOL GetWindowVisible(VOID);
	HWND GetWindowHandle(VOID) { return m_Hwnd; }

	UINT GetWidth(VOID) const { return m_PresentParameters.BackBufferWidth; }
	UINT GetHeight(VOID) const { return m_PresentParameters.BackBufferHeight; }

	BOOL GetBlur(VOID) const { return m_Blur; }

	FLOAT GetLevel(UINT index) const { return m_Levels[index]; }
	FLOAT GetWaveform(UINT index) const { return m_Waveform[index]; }
	CONST PEAK& GetPeak(UINT index) const { return m_Peaks[index]; }

	DOUBLE GetMagnitude(double level,double scale,double magnitude,double cutrange,double sharpness,unsigned long x,unsigned long size);

	VOID SetColorBackground(const Color& color) { m_Background = color; }
	CONST Color& GetColorBackground(void) const { return m_Background; }

	VOID SetColorTop(const Color& color) { m_Top = color; }
	CONST Color& GetColorTop(void) const { return m_Top; }

	VOID SetColorTopEnd(const Color& color) { m_TopEnd = color; }
	CONST Color& GetColorTopEnd(void) const { return m_TopEnd; }

	VOID SetColorBottom(const Color& color) { m_Bottom = color; }
	CONST Color& GetColorBottom(void) const { return m_Bottom; }

	VOID SetColorBottomEnd(const Color& color) { m_BottomEnd = color; }
	CONST Color& GetColorBottomEnd(void) const { return m_BottomEnd; }

	VOID SetColorPeak(const Color& color) { m_Peak = color; }
	CONST Color& GetColorPeak(void) const { return m_Peak; }

	CRITICAL_SECTION& GetUpdateSection(VOID) { return m_UpdateSection; }

protected:
	D3DADAPTER_IDENTIFIER9	m_AdapterIdentifier;
	D3DPRESENT_PARAMETERS	m_PresentParameters;
	D3DCAPS9				m_Caps;

	CRITICAL_SECTION		m_UpdateSection;
	
	LPDIRECT3D9				m_Direct3D;
	LPDIRECT3DDEVICE9		m_Device;
	LPDIRECT3DVERTEXBUFFER9	m_SpriteBuffer;
	LPDIRECT3DVERTEXBUFFER9	m_LineBuffer;
	LPDIRECT3DTEXTURE9		m_SpriteTexture;
	LPSPRITEVERTEX			m_SpriteData;
	LPLINEVERTEX			m_LineData;
	ULONG					m_SpriteCount;
	ULONG					m_LineCount;
	LPDIRECT3DTEXTURE9		m_BlurTexture;
	LPDIRECT3DTEXTURE9		m_VisualizationTexture;

	BOOL					m_NeedReset;
	BOOL					m_Blur;

	Array<CVisualization*>			m_Visualizations;
	ULONG							m_Visualization;

	LONGLONG						m_TimerFrequency;
	LONGLONG						m_TimerLast;
	DOUBLE							m_TimerElapsed;
	DOUBLE							m_TimerAbsolute;
	ULONG							m_TimerFrameRate;

	PFLOAT							m_LevelsBuffer;
	PFLOAT							m_Levels;
	PFLOAT							m_WaveformBuffer;
	PFLOAT							m_Waveform;
	PEAK							m_Peaks[VISUALIZATION_BARCOUNT];

	FLOAT							m_PeakTimeout;
	FLOAT							m_PeakGravity;
	FLOAT							m_Speed;

	HWND							m_Hwnd;
	HANDLE							m_Renderer;
	ULONG							m_RendererId;

	HMODULE							m_Direct3DLibrary;
	LPDIRECT3DCREATE9				m_Direct3DCreate9;
	LPDIRECT3DSETREGION				m_Direct3DSetRegion;
	LPDIRECT3DBEGINEVENT			m_Direct3DBeginEvent;
	LPDIRECT3DENDEVENT				m_Direct3DEndEvent;

	Color							m_Background;
	Color							m_Top;
	Color							m_TopEnd;
	Color							m_Bottom;
	Color							m_BottomEnd;
	Color							m_Peak;
};

ULONG WINAPI RendererThread(CGraphics* graphics);