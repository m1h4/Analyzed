#include "Globals.h"
#include "Visualizations.h"

#pragma warning(disable : 4244)	// Conversions (double to float etc.)

void WriteGhostColor(CGraphics& graphics,const Rect& rect,const Color& colortl,const Color& colortr,const Color& colorbr,const Color& colorbl,IDirect3DTexture9* texture)
{
	long width = 1;

	// Top
	graphics.SpriteWrite(Rect(rect.left,rect.top,rect.right,rect.top + width),colortl,colortr,colortl,colorbl,texture);

	// Bottom
	graphics.SpriteWrite(Rect(rect.left,rect.bottom - width,rect.right,rect.bottom),colortl,colortr,colortl,colorbl,texture);

	// Left
	graphics.SpriteWrite(Rect(rect.left,rect.top + width,rect.left + width,rect.bottom - width),colortl,colortr,colortl,colorbl,texture);

	// Right
	graphics.SpriteWrite(Rect(rect.right - width,rect.top + width,rect.right,rect.bottom - width),colortl,colortr,colortl,colorbl,texture);
}

void WriteGhost(CGraphics& graphics,const Rect& rect,const Color& color,IDirect3DTexture9* texture)
{
	WriteGhostColor(graphics,rect,color,color,color,color,texture);
}

void WriteColor(CGraphics& graphics,const Rect& rect,const Color& colortl,const Color& colortr,const Color& colorbr,const Color& colorbl,IDirect3DTexture9* texture)
{
	graphics.SpriteWrite(rect,colortl,colortr,colorbr,colorbl,texture);
}

void Write(CGraphics& graphics,const Rect& rect,const Color& color,IDirect3DTexture9* texture)
{
	WriteColor(graphics,rect,color,color,color,color,texture);
}

typedef VOID (*LPWRITE)(CGraphics& gr,const Rect& rect,const Color& color,IDirect3DTexture9* texture);
typedef VOID (*LPWRITECOLOR)(CGraphics& gr,const Rect& rect,const Color& colortl,const Color& colortr,const Color& colorbr,const Color& colorbl,IDirect3DTexture9* texture);

VOID DrawBars(CGraphics& graphics,LPWRITE write,LPWRITECOLOR writeColor)
{
	Rect rect;
	FLOAT factor = (FLOAT)graphics.GetHeight() / 384.0f;
	FLOAT width = (FLOAT)(graphics.GetWidth() - 2) / (FLOAT)VISUALIZATION_BARCOUNT;
	FLOAT height = width / 4.0f;
	FLOAT margin = 2.0f * factor;

	// Draw the bar visualization
	for(UINT i = 0; i < VISUALIZATION_BARCOUNT; ++i)
	{
		Color dbottom(graphics.GetColorBottom());
		dbottom.LinearInterp(graphics.GetColorBottomEnd(),(float)i/(float)VISUALIZATION_BARCOUNT);

		Color dtop(graphics.GetColorTop());
		dtop.LinearInterp(graphics.GetColorTopEnd(),(float)i/(float)VISUALIZATION_BARCOUNT);

		rect.left = margin + width * i;
		rect.bottom = graphics.GetHeight() - 1 - 1;
		rect.top = rect.bottom - graphics.GetLevel(i) * factor;
		rect.right = rect.left + width * 0.8f;

		// Draw the bar
		Color dbtop(dbottom);
		dbtop.LinearInterp(dtop,(float)(rect.bottom-rect.top)/(float)graphics.GetHeight());

		//graphics->SpriteWrite(rect,dbtop,dbtop,dbottom,dbottom,NULL);
		writeColor(graphics,rect,dbtop,dbtop,dbottom,dbottom,NULL);

		rect.left = margin + width * i;
		rect.bottom = graphics.GetHeight() - 1 - 1 - graphics.GetPeak(i).position * factor;
		rect.top = rect.bottom - height;
		rect.right = rect.left + width * 0.8f;

		// Draw the peak
		//graphics->SpriteWrite(rect,graphics->GetColorPeak(),NULL);
		write(graphics,rect,graphics.GetColorPeak(),NULL);
	}
}

HRESULT CBars::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	DrawBars(graphics,Write,WriteColor);

	return S_OK;
}

HRESULT CGhostBars::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	DrawBars(graphics,WriteGhost,WriteGhostColor);

	return S_OK;
}

VOID DrawDoubleBars(CGraphics& graphics,LPWRITE write,LPWRITECOLOR writeColor)
{
	Rect rect;
	FLOAT factor = (FLOAT)graphics.GetHeight() / 384.0f;
	FLOAT width = (FLOAT)(graphics.GetWidth() - 2) / (FLOAT)VISUALIZATION_BARCOUNT;
	FLOAT height = width / 4.0f;
	FLOAT margin = 2.0f * factor;

	// Draw the bar visualization
	for(UINT i = 0; i < VISUALIZATION_BARCOUNT; ++i)
	{
		Color dbottom(graphics.GetColorBottom());
		dbottom.LinearInterp(graphics.GetColorBottomEnd(),(float)i/(float)VISUALIZATION_BARCOUNT);

		Color dtop(graphics.GetColorTop());
		dtop.LinearInterp(graphics.GetColorTopEnd(),(float)i/(float)VISUALIZATION_BARCOUNT);

		rect.left = margin + width * i;
		rect.bottom = graphics.GetHeight() / 2.0f + (graphics.GetLevel(i) * factor) / 2.0f;
		rect.top = rect.bottom - graphics.GetLevel(i) * factor;
		rect.right = rect.left + width * 0.8f;

		// Draw the bar
		Color dbtop(dbottom);
		dbtop.LinearInterp(dtop,(float)(rect.bottom-rect.top)/(float)graphics.GetHeight());

		//graphics->SpriteWrite(rect,dbtop,dbtop,dbottom,dbottom,NULL);
		writeColor(graphics,rect,dbtop,dbtop,dbottom,dbottom,NULL);

		rect.left = margin + width * i;
		rect.bottom = graphics.GetHeight() / 2.0f - (graphics.GetPeak(i).position * factor) / 2.0f;
		rect.top = rect.bottom - height;
		rect.right = rect.left + width * 0.8f;

		// Draw the peak
		//graphics->SpriteWrite(rect,graphics->GetColorPeak(),NULL);
		write(graphics,rect,graphics.GetColorPeak(),NULL);

		rect.left = margin + width * i;
		rect.bottom = graphics.GetHeight() / 2.0f + (graphics.GetPeak(i).position * factor) / 2.0f + height;
		rect.top = rect.bottom - height;
		rect.right = rect.left + width * 0.8f;

		// Draw the peak
		//graphics->SpriteWrite(rect,graphics->GetColorPeak(),NULL);
		write(graphics,rect,graphics.GetColorPeak(),NULL);
	}
}

HRESULT CDoubleBars::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	DrawDoubleBars(graphics,Write,WriteColor);

	return S_OK;
}

HRESULT CDoubleGhostBars::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	DrawDoubleBars(graphics,WriteGhost,WriteGhostColor);

	return S_OK;
}

VOID DrawDigitalBars(CGraphics& graphics,LPWRITE write,LPWRITECOLOR writeColor)
{
	FLOAT factor = (FLOAT)graphics.GetHeight() / 384.0f;
	FLOAT width = (FLOAT)(graphics.GetWidth() - 2) / (FLOAT)VISUALIZATION_BARCOUNT;
	FLOAT height = width / 4.0f;
	FLOAT margin = 2.0f * factor;
	FLOAT realwidth = 0.8f;

	FLOAT totalHeightBars = (FLOAT)graphics.GetHeight()*factor / (height + margin);
	FLOAT heightFactor = factor / (height + margin);

	for(UINT i = 0; i < VISUALIZATION_BARCOUNT; ++i)
	{
		Color dbottom(graphics.GetColorBottom());
		dbottom.LinearInterp(graphics.GetColorBottomEnd(),(float)i/(float)VISUALIZATION_BARCOUNT);

		Color dtop(graphics.GetColorTop());
		dtop.LinearInterp(graphics.GetColorTopEnd(),(float)i/(float)VISUALIZATION_BARCOUNT);

		for(UINT y = 0; y < graphics.GetLevel(i) * heightFactor; ++y)
		{
			Rect rect;
			rect.left = margin + width * i;
			rect.bottom = graphics.GetHeight() - y * (height + margin) - margin;
			rect.top = rect.bottom - height;
			rect.right = rect.left + width * realwidth;

			Color dbtop(dbottom);
			dbtop.LinearInterp(dtop,(float)rect.top/(float)graphics.GetHeight());

			dbtop.a = min(graphics.GetLevel(i) * heightFactor - y,1.0f);

			// Draw the peak
			//graphics->SpriteWrite(rect,dbtop,NULL);
			write(graphics,rect,dbtop,NULL);
		}

		Rect rect;
		rect.left = margin + width * i;
		rect.bottom = graphics.GetHeight() - UINT(graphics.GetPeak(i).position * heightFactor) * (height + margin) - margin;
		rect.top = rect.bottom - height;
		rect.right = rect.left + width * realwidth;

		Color dbbottom(dtop);
		dbbottom.LinearInterp(dbottom,(float)rect.top/(float)graphics.GetHeight());

		// Draw the peak
		//graphics->SpriteWrite(rect,graphics->GetColorPeak(),NULL);
		write(graphics,rect,graphics.GetColorPeak(),NULL);
	}
}

HRESULT CDigitalBars::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	DrawDigitalBars(graphics,Write,WriteColor);

	return S_OK;
}

HRESULT CDigitalGhostBars::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	DrawDigitalBars(graphics,WriteGhost,WriteGhostColor);

	return S_OK;
}

HRESULT CDigitalScope::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	FLOAT factor = (FLOAT)graphics.GetHeight() / 384.0f;
	FLOAT width = (FLOAT)(graphics.GetWidth() - 2) / (FLOAT)(SA_BUFFER_SIZE/8);
	FLOAT height = width / 2.0f;
	FLOAT realwidth = 0.8f;

	for(UINT i = 0; i < (SA_BUFFER_SIZE/4); ++i)
	{
		Color dbottom(graphics.GetColorBottom());
		dbottom.LinearInterp(graphics.GetColorBottomEnd(),(float)i/(float)(SA_BUFFER_SIZE/4));

		Color dtop(graphics.GetColorTop());
		dtop.LinearInterp(graphics.GetColorTopEnd(),(float)i/(float)(SA_BUFFER_SIZE/4));

		Rect rect;
		rect.left = width * 0.2f + (width/2) * i;
		rect.bottom = graphics.GetHeight() / 2 - UINT(graphics.GetWaveform(i*4) * factor);
		rect.top = rect.bottom - height;
		rect.right = rect.left + width * realwidth;

		Color dbbottom(dtop);
		dbbottom.LinearInterp(dbottom,(float)rect.top/(float)graphics.GetHeight());

		// Draw the peak
		graphics.SpriteWrite(rect,dbbottom,NULL);
	}

	return S_OK;
}

HRESULT CBarScope::Draw(CGraphics& graphics)
{
	AUTO_SECTION(graphics.GetUpdateSection());

	Rect rect;
	FLOAT factor = (FLOAT)graphics.GetHeight() / 384.0f;
	FLOAT width = (FLOAT)(graphics.GetWidth() - 2) / (FLOAT)(SA_BUFFER_SIZE/8);
	FLOAT height = width / 4.0f;

	// Draw the bar visualization
	for(UINT i = 0; i < (SA_BUFFER_SIZE/8); ++i)
	{
		Color dbottom(graphics.GetColorBottom());
		dbottom.LinearInterp(graphics.GetColorBottomEnd(),(float)i/(float)(SA_BUFFER_SIZE/8));

		Color dtop(graphics.GetColorTop());
		dtop.LinearInterp(graphics.GetColorTopEnd(),(float)i/(float)(SA_BUFFER_SIZE/8));

		rect.left = width * 0.2f + width * i;
		rect.bottom = graphics.GetHeight() / 2 + UINT(abs(graphics.GetWaveform(i*8)) * factor);
		rect.top = graphics.GetHeight() / 2 - UINT(abs(graphics.GetWaveform(i*8)) * factor);
		rect.right = rect.left + width * 0.8f;

		if(rect.bottom < rect.top)
			rect.bottom ^= rect.top ^= rect.bottom ^= rect.top;

		// Draw the bar
		Color dbtop(dbottom);
		dbtop.LinearInterp(dtop,(float)(rect.bottom-rect.top)/(float)graphics.GetHeight());

		graphics.SpriteWrite(rect,dbtop,dbtop,dbottom,dbottom,NULL);
	}

	return S_OK;
}