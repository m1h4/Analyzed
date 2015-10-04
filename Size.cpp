#include "Globals.h"
#include "Size.h"

Size::Size(void)
{
}

Size::Size(long cx,long cy)
{
	Set(cx,cy);
}

Size::Size(long c)
{
	Set(c);
}

Size::Size(const long* m)
{
	Set(m);
}

Size::Size(const Size& other)
{
	Set(other);
}

Size::Size(const SizeF& other)
{
	Set(other);
}

void Size::Set(long cx,long cy)
{
	this->cx = cx;
	this->cy = cy;
}

void Size::Set(long c)
{
	Set(c,c);
}

void Size::Set(const long* m)
{
	this->m[0] = m[0];
	this->m[1] = m[1];
}

void Size::Set(const Size& other)
{
	cx = other.cx;
	cy = other.cy;
}

void Size::Set(const SizeF& other)
{
	#pragma message("Warning: Conversion from SizeF disabled.\n")
	ASSERT(false);
	//cx = other.cx;
	//cy = other.cy;
}

void Size::SetEmpty(void)
{
	cx = cy = 0;
}

bool Size::Empty(void) const
{
	return cx <= 0 && cy <= 0;
}

void Size::Expand(long dx,long dy)
{
	cx += dx;
	cy += dy;
}

void Size::Expand(long d)
{
	Expand(d,d);
}

bool Size::operator==(const Size& other) const
{
	return cx == other.cx && cy == other.cy;
}

bool Size::operator!=(const Size& other) const
{
	return cx != other.cx && cy != other.cy;
}

Size Size::operator+(const Size& other) const
{
	return Size(cx+other.cx,cy+other.cy);
}

Size Size::operator-(const Size& other) const
{
	return Size(cx-other.cx,cy-other.cy);
}

Size Size::operator+(long d) const
{
	return Size(cx+d,cy+d);
}

Size Size::operator-(long d) const
{
	return Size(cx-d,cy-d);
}

Size& Size::operator+=(const Size& other)
{
	cx += other.cx;
	cy += other.cy;
	
	return *this;
}

Size& Size::operator-=(const Size& other)
{
	cx -= other.cx;
	cy -= other.cy;
	
	return *this;
}

Size& Size::operator+=(long d)
{
	cx += d;
	cy += d;

	return *this;
}

Size& Size::operator-=(long d)
{
	cx -= d;
	cy -= d;

	return *this;
}

SizeF::SizeF(void)
{
}

SizeF::SizeF(float cx,float cy)
{
	Set(cx,cy);
}

SizeF::SizeF(float c)
{
	Set(c);
}

SizeF::SizeF(const float* m)
{
	Set(m);
}

SizeF::SizeF(const SizeF& other)
{
	Set(other);
}

SizeF::SizeF(const Size& other)
{
	Set(other);
}

void SizeF::Set(float cx,float cy)
{
	this->cx = cx;
	this->cy = cy;
}

void SizeF::Set(float c)
{
	Set(c,c);
}

void SizeF::Set(const float* m)
{
	this->m[0] = m[0];
	this->m[1] = m[1];
}

void SizeF::Set(const SizeF& other)
{
	cx = other.cx;
	cy = other.cy;
}

void SizeF::Set(const Size& other)
{
	cx = other.cx;
	cy = other.cy;
}

void SizeF::SetEmpty(void)
{
	cx = cy = 0.0f;
}

bool SizeF::Empty(void) const
{
	return cx <= 0.0f && cy <= 0.0f;
}

void SizeF::Expand(float dx,float dy)
{
	cx += dx;
	cy += dy;
}

void SizeF::Expand(float d)
{
	Expand(d,d);
}

bool SizeF::operator==(const SizeF& other) const
{
	return cx == other.cx && cy == other.cy;
}

bool SizeF::operator!=(const SizeF& other) const
{
	return cx != other.cx && cy != other.cy;
}

SizeF SizeF::operator+(const SizeF& other) const
{
	return SizeF(cx+other.cx,cy+other.cy);
}

SizeF SizeF::operator-(const SizeF& other) const
{
	return SizeF(cx-other.cx,cy-other.cy);
}

SizeF SizeF::operator+(float d) const
{
	return SizeF(cx+d,cy+d);
}

SizeF SizeF::operator-(float d) const
{
	return SizeF(cx-d,cy-d);
}

SizeF& SizeF::operator+=(const SizeF& other)
{
	cx += other.cx;
	cy += other.cy;
	
	return *this;
}

SizeF& SizeF::operator-=(const SizeF& other)
{
	cx -= other.cx;
	cy -= other.cy;
	
	return *this;
}

SizeF& SizeF::operator+=(float d)
{
	cx += d;
	cy += d;

	return *this;
}

SizeF& SizeF::operator-=(float d)
{
	cx -= d;
	cy -= d;

	return *this;
}