#include "Globals.h"
#include "Rect.h"

Rect::Rect(void)
{
}

Rect::Rect(long left,long top,long right,long bottom)
{
	Set(left,top,right,bottom);
}

Rect::Rect(long width,long height)
{
	Set(width,height);
}

Rect::Rect(const Point& topLeft,const Size& size)
{
	Set(topLeft,size);
}

Rect::Rect(const Point& topLeft,const Point& bottomRight)
{
	Set(topLeft,bottomRight);
}

Rect::Rect(const long* m)
{
	Set(m);
}

Rect::Rect(const Rect& other)
{
	Set(other);
}

Rect::Rect(const RectF& other)
{
	Set(other);
}

long Rect::GetWidth(void) const
{
	return right - left;
}

long Rect::GetHeight(void) const
{
	return bottom - top;
}

void Rect::SetWidth(long width)
{
	right = left + width;
}

void Rect::SetHeight(long height)
{
	bottom = top + height;
}

void Rect::SetWidthRight(long width)
{
	left = right - width;
}

void Rect::SetHeightRight(long height)
{
	top = bottom - height;
}

void Rect::Set(long left,long top,long right,long bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

void Rect::Set(long width,long height)
{
	left = 0;
	top = 0;

	right = width;
	bottom = height;
}

void Rect::Set(const Point& topLeft,const Size& size)
{
	left = topLeft.x;
	top = topLeft.y;

	right = left + size.cx;
	bottom = top + size.cy;
}

void Rect::Set(const Point& topLeft,const Point& bottomRight)
{
	left = topLeft.x;
	top = topLeft.y;

	right = bottomRight.x;
	bottom = bottomRight.y;
}

void Rect::Set(const long* m)
{
	this->m[0] = m[0];
	this->m[1] = m[1];
	this->m[2] = m[2];
	this->m[3] = m[3];
}

void Rect::Set(const Rect& other)
{
	left = other.left;
	top = other.top;
	right = other.right;
	bottom = other.bottom;
}

void Rect::Set(const RectF& other)
{
	#pragma message("Warning: Conversion from RectF disabled.")
	ASSERT(false);
	//left = other.left;
	//top = other.top;
	//right = other.right;
	//bottom = other.bottom;
}

void Rect::SetEmpty(void)
{
	left = top = right = bottom = 0;
}

bool Rect::Empty(void) const
{
	if(right <= left)
		return true;
	
	if(bottom <= top)
		return true;
	
	return false;
}

void Rect::AdjustWidth(void)
{
	if(right < left)
	{
		long tmp = left;
		left = right;
		right = tmp;
	}
}

void Rect::AdjustHeight(void)
{
	if(bottom < top)
	{
		long tmp = top;
		top = bottom;
		bottom = tmp;
	}
}

void Rect::Adjust(void)
{
	AdjustWidth();
	AdjustHeight();
}

void Rect::Inflate(long dx,long dy)
{
	left -= dx;
	right += dx;
	top -= dy;
	bottom += dy;
}

void Rect::Inflate(long d)
{
	Inflate(d,d);
}

void Rect::Offset(long dx,long dy)
{
	left += dx;
	right += dx;
	top += dy;
	bottom += dy;
}

void Rect::Offset(long d)
{
	this->Offset(d,d);
}

void Rect::Scale(float dx,float dy)
{
	#pragma message("Warning: Scale disabled.")
	ASSERT(false);
	//long cx = (right + left) / 2;
	//long cy = (bottom + top) / 2;
	
	//long width = (right - left) / 2;
	//long height = (bottom - top) / 2;
	
	//left = (long)(cx - width * dx);
	//right = (long)(cx + width * dx);
	//top = (long)(cy - height * dy);
	//bottom = (long)(cy + height * dy);
}

void Rect::Scale(float d)
{
	Scale(d,d);
}

bool Rect::PointIn(const Point& point) const
{
	if(point.x < left || point.x > right)
		return false;
	
	if(point.y < top || point.y > bottom)
		return false;
	
	return true;
}

Point Rect::GetCenter(void) const
{
	return Point((right+left)/2,(bottom+top)/2);
}

// TODO You should check if the result rect is valid if there is a chance that the two rects don't intersect!
void Rect::Intersect(const Rect& other)
{
	left = max(left,other.left);
	right = min(right,other.right);
	top = max(top,other.top);
	bottom = min(bottom,other.bottom);
	
	//return RectIsEmpty(*rect);
}

Rect Rect::operator+(const Point& point) const
{
	return Rect(left+point.x,top+point.y,right+point.x,bottom+point.y);
}

Rect Rect::operator-(const Point& point) const
{
	return Rect(left-point.x,top-point.y,right-point.x,bottom-point.y);
}

Rect Rect::operator+(long d) const
{
	return Rect(left+d,top+d,right+d,bottom+d);
}

Rect Rect::operator-(long d) const
{
	return Rect(left-d,top-d,right-d,bottom-d);
}

Rect& Rect::operator+=(const Point& point)
{
	left += point.x;
	top += point.y;
	right += point.x;
	bottom += point.y;
	
	return *this;
}

Rect& Rect::operator-=(const Point& point)
{
	left -= point.x;
	top -= point.y;
	right -= point.x;
	bottom -= point.y;
	
	return *this;
}

Rect& Rect::operator+=(long d)
{
	left += d;
	top += d;
	right += d;
	bottom += d;
	
	return *this;
}

Rect& Rect::operator-=(long d)
{
	left -= d;
	top -= d;
	right -= d;
	bottom -= d;
	
	return *this;
}

bool Rect::operator==(const Rect& other) const
{
	return left == other.left && top == other.top && right == other.right && bottom == other.bottom;
}

bool Rect::operator!=(const Rect& other) const
{
	return left != other.left && top != other.top && right != other.right && bottom != other.bottom;
}

RectF::RectF(void)
{
}

RectF::RectF(float left,float top,float right,float bottom)
{
	Set(left,top,right,bottom);
}

RectF::RectF(float width,float height)
{
	Set(width,height);
}

RectF::RectF(const PointF& topLeft,const SizeF& size)
{
	Set(topLeft,size);
}

RectF::RectF(const PointF& topLeft,const PointF& bottomRight)
{
	Set(topLeft,bottomRight);
}

RectF::RectF(const float* m)
{
	Set(m);
}

RectF::RectF(const RectF& other)
{
	Set(other);
}

RectF::RectF(const Rect& other)
{
	Set(other);
}

float RectF::GetWidth(void) const
{
	return right - left;
}

float RectF::GetHeight(void) const
{
	return bottom - top;
}

void RectF::SetWidth(float width)
{
	right = left + width;
}

void RectF::SetHeight(float height)
{
	bottom = top + height;
}

void RectF::SetWidthRight(float width)
{
	left = right - width;
}

void RectF::SetHeightRight(float height)
{
	top = bottom - height;
}

void RectF::Set(float left,float top,float right,float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

void RectF::Set(float width,float height)
{
	left = 0.0f;
	top = 0.0f;

	right = width;
	bottom = height;
}

void RectF::Set(const PointF& topLeft,const SizeF& size)
{
	left = topLeft.x;
	top = topLeft.y;

	right = left + size.cx;
	bottom = top + size.cy;
}

void RectF::Set(const PointF& topLeft,const PointF& bottomRight)
{
	left = topLeft.x;
	top = topLeft.y;

	right = bottomRight.x;
	bottom = bottomRight.y;
}

void RectF::Set(const float* m)
{
	this->m[0] = m[0];
	this->m[1] = m[1];
	this->m[2] = m[2];
	this->m[3] = m[3];
}

void RectF::Set(const RectF& other)
{
	left = other.left;
	top = other.top;
	right = other.right;
	bottom = other.bottom;
}

void RectF::Set(const Rect& other)
{
	left = (float)other.left;
	top = (float)other.top;
	right = (float)other.right;
	bottom = (float)other.bottom;
}

void RectF::SetEmpty(void)
{
	left = top = right = bottom = 0.0f;
}

bool RectF::Empty(void) const
{
	if(right <= left)
		return true;
	
	if(bottom <= top)
		return true;
	
	return false;
}

void RectF::AdjustWidth(void)
{
	if(right < left)
		*(unsigned int*)&left ^= *(unsigned int*)&right ^= *(unsigned int*)&left ^= *(unsigned int*)&right;
}

void RectF::AdjustHeight(void)
{
	if(bottom < top)
		*(unsigned int*)&top ^= *(unsigned int*)&bottom ^= *(unsigned int*)&top ^= *(unsigned int*)&bottom;
}

void RectF::Adjust(void)
{
	AdjustWidth();
	AdjustHeight();
}

void RectF::Inflate(float dx,float dy)
{
	left -= dx;
	right += dx;
	top -= dy;
	bottom += dy;
}

void RectF::Inflate(float d)
{
	Inflate(d,d);
}

void RectF::Offset(float dx,float dy)
{
	left += dx;
	right += dx;
	top += dy;
	bottom += dy;
}

void RectF::Offset(float d)
{
	this->Offset(d,d);
}

void RectF::Scale(float dx,float dy)
{
	float cx = (right + left) / 2.0f;
	float cy = (bottom + top) / 2.0f;
	
	float width = (right - left) / 2.0f;
	float height = (bottom - top) / 2.0f;
	
	left = cx - width * dx;
	right = cx + width * dx;
	top = cy - height * dy;
	bottom = cy + height * dy;
}

void RectF::Scale(float d)
{
	Scale(d,d);
}

bool RectF::PointIn(const PointF& point) const
{
	if(point.x < left || point.x > right)
		return false;
	
	if(point.y < top || point.y > bottom)
		return false;
	
	return true;
}

PointF RectF::GetCenter(void) const
{
	return PointF((right+left)/2.0f,(bottom+top)/2.0f);
}

// TODO You should check if the result RectF is valid if there is a chance that the two RectFs don't intersect!
void RectF::Intersect(const RectF& other)
{
	left = max(left,other.left);
	right = min(right,other.right);
	top = max(top,other.top);
	bottom = min(bottom,other.bottom);
	
	//return RectFIsEmpty(*RectF);
}

RectF RectF::operator+(const PointF& point) const
{
	return RectF(left+point.x,top+point.y,right+point.x,bottom+point.y);
}

RectF RectF::operator-(const PointF& point) const
{
	return RectF(left-point.x,top-point.y,right-point.x,bottom-point.y);
}

RectF RectF::operator+(float d) const
{
	return RectF(left+d,top+d,right+d,bottom+d);
}

RectF RectF::operator-(float d) const
{
	return RectF(left-d,top-d,right-d,bottom-d);
}

RectF& RectF::operator+=(const PointF& point)
{
	left += point.x;
	top += point.y;
	right += point.x;
	bottom += point.y;
	
	return *this;
}

RectF& RectF::operator-=(const PointF& point)
{
	left -= point.x;
	top -= point.y;
	right -= point.x;
	bottom -= point.y;
	
	return *this;
}

RectF& RectF::operator+=(float d)
{
	left += d;
	top += d;
	right += d;
	bottom += d;
	
	return *this;
}

RectF& RectF::operator-=(float d)
{
	left -= d;
	top -= d;
	right -= d;
	bottom -= d;
	
	return *this;
}

bool RectF::operator==(const RectF& other) const
{
	return left == other.left && top == other.top && right == other.right && bottom == other.bottom;
}

bool RectF::operator!=(const RectF& other) const
{
	return left != other.left && top != other.top && right != other.right && bottom != other.bottom;
}
