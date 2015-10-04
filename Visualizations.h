#pragma once

#include "Graphics.h"

class CBars : public CVisualization
{
public:
	CBars(void) : CVisualization(IDS_PRESETBARS) {}

	HRESULT Draw(CGraphics& graphics);
};

class CGhostBars : public CVisualization
{
public:
	CGhostBars(void) : CVisualization(IDS_PRESETGHOSTBARS) {}

	HRESULT Draw(CGraphics& graphics);
};

class CDoubleBars : public CVisualization
{
public:
	CDoubleBars(void) : CVisualization(IDS_PRESETDOUBLEBARS) {}

	HRESULT Draw(CGraphics& graphics);
};

class CDoubleGhostBars : public CVisualization
{
public:
	CDoubleGhostBars(void) : CVisualization(IDS_PRESETDOUBLEGHOSTBARS) {}

	HRESULT Draw(CGraphics& graphics);
};

class CDigitalBars : public CVisualization
{
public:
	CDigitalBars(void) : CVisualization(IDS_PRESETDIGITALBARS) {}

	HRESULT Draw(CGraphics& graphics);
};

class CDigitalGhostBars : public CVisualization
{
public:
	CDigitalGhostBars(void) : CVisualization(IDS_PRESETDIGITALGHOSTBARS) {}

	HRESULT Draw(CGraphics& graphics);
};

class CBarScope : public CVisualization
{
public:
	CBarScope(void) : CVisualization(IDS_PRESETBARSCOPE) {}

	HRESULT Draw(CGraphics& graphics);
};

class CDigitalScope : public CVisualization
{
public:
	CDigitalScope(void) : CVisualization(IDS_PRESETDIGITALSCOPE) {}

	HRESULT Draw(CGraphics& graphics);
};