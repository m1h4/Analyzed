#include "Globals.h"
#include "Point.h"

Point::Point(void)
{
}

Point::Point(long x,long y)
{
	Set(x,y);
}

Point::Point(long xy)
{ 
	Set(xy);
}

Point::Point(const long* m)
{
	Set(m);
}

Point::Point(const Point& other)
{
	Set(other);
}

Point::Point(const PointF& other)
{
	Set(other);
}

void Point::Set(long x,long y)
{
	this->x = x;
	this->y = y;
}

void Point::Set(long xy)
{
	x = xy;
	y = xy;
}

void Point::Set(const long* m)
{
	this->m[0] = m[0];
	this->m[1] = m[1];
}

void Point::Set(const Point& other)
{
	x = other.x;
	y = other.y;
}

void Point::Set(const PointF& other)
{
	#pragma message("Warning: Conversion from PointF disabled.")
	ASSERT(false);
	//x = other.x;
	//y = other.y;
}

void Point::SetEmpty(void)
{
	x = y = 0;
}

bool Point::Empty(void) const
{
	return x == 0 && y == 0;
}

void Point::Offset(long dx,long dy)
{
	x += dx;
	y += dy;
}

void Point::Offset(long d)
{
	Offset(d,d);
}

bool Point::operator==(const Point& other) const
{
	return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const
{
	return x != other.x && y != other.y;
}

Point Point::operator+(const Point& other) const
{
	return Point(x+other.x,y+other.y);
}

Point Point::operator-(const Point& other) const
{
	return Point(x-other.x,y-other.y);
}

Point Point::operator+(long d) const
{
	return Point(x+d,y+d);
}

Point Point::operator-(long d) const
{
	return Point(x-d,y-d);
}

Point& Point::operator+=(const Point& other)
{
	x += other.x;
	y += other.y;
	
	return *this;
}

Point& Point::operator-=(const Point& other)
{
	x -= other.x;
	y -= other.y;
		
	return *this;
}

Point& Point::operator+=(long d)
{
	x += d;
	y += d;

	return *this;
}

Point& Point::operator-=(long d)
{
	x -= d;
	y -= d;

	return *this;
}

PointF::PointF(void)
{
}

PointF::PointF(float x,float y)
{
	Set(x,y);
}

PointF::PointF(float xy)
{ 
	Set(xy);
}

PointF::PointF(const float* m)
{
	Set(m);
}

PointF::PointF(const PointF& other)
{
	Set(other);
}

PointF::PointF(const Point& other)
{
	Set(other);
}

void PointF::Set(float x,float y)
{
	this->x = x;
	this->y = y;
}

void PointF::Set(float xy)
{
	x = xy;
	y = xy;
}

void PointF::Set(const float* m)
{
	this->m[0] = m[0];
	this->m[1] = m[1];
}

void PointF::Set(const PointF& other)
{
	x = other.x;
	y = other.y;
}

void PointF::Set(const Point& other)
{
	x = (float)other.x;
	y = (float)other.y;
}

void PointF::SetEmpty(void)
{
	x = y = 0;
}

bool PointF::Empty(void) const
{
	return x == 0.0f && y == 0.0f;
}

void PointF::Offset(float dx,float dy)
{
	x += dx;
	y += dy;
}

void PointF::Offset(float d)
{
	Offset(d,d);
}

bool PointF::operator==(const PointF& other) const
{
	return x == other.x && y == other.y;
}

bool PointF::operator!=(const PointF& other) const
{
	return x != other.x && y != other.y;
}

PointF PointF::operator+(const PointF& other) const
{
	return PointF(x+other.x,y+other.y);
}

PointF PointF::operator-(const PointF& other) const
{
	return PointF(x-other.x,y-other.y);
}

PointF PointF::operator+(float d) const
{
	return PointF(x+d,y+d);
}

PointF PointF::operator-(float d) const
{
	return PointF(x-d,y-d);
}

PointF& PointF::operator+=(const PointF& other)
{
	x += other.x;
	y += other.y;
	
	return *this;
}

PointF& PointF::operator-=(const PointF& other)
{
	x -= other.x;
	y -= other.y;
		
	return *this;
}

PointF& PointF::operator+=(float d)
{
	x += d;
	y += d;

	return *this;
}

PointF& PointF::operator-=(float d)
{
	x -= d;
	y -= d;

	return *this;
}