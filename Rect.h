#pragma once

#include "Point.h"
#include "Size.h"

class Rect;
class RectF;

class Rect
{
public:
	Rect(void);
	Rect(long left,long top,long right,long bottom);
	Rect(long width,long height);
	Rect(const Point& topLeft,const Size& size);
	Rect(const Point& topLeft,const Point& bottomRight);
	Rect(const long* m);
	Rect(const Rect& other);
	explicit Rect(const RectF& other);

	long GetWidth(void) const;
	long GetHeight(void) const;

	void SetWidth(long width);
	void SetHeight(long height);
	void SetWidthRight(long width);
	void SetHeightRight(long height);

	void Set(long left,long top,long right,long bottom);
	void Set(long width,long height);
	void Set(const Point& topLeft,const Size& size);
	void Set(const Point& topLeft,const Point& bottomRight);
	void Set(const long* m);
	void Set(const Rect& other);
	void Set(const RectF& other);

	void SetEmpty(void);
	bool Empty(void) const;

	void AdjustWidth(void);
	void AdjustHeight(void);
	void Adjust(void);

	void Inflate(long dx,long dy);
	void Inflate(long d);

	void Offset(long dx,long dy);
	void Offset(long d);

	void Scale(float dx,float dy);
	void Scale(float d);

	bool PointIn(const Point& point) const;

	Point GetCenter(void) const;

	// TODO You should check if the result rect is valid if there is a chance that the two rects don't intersect!
	void Intersect(const Rect& other);

	Rect operator+(const Point& point) const;
	Rect operator-(const Point& point) const;

	Rect operator+(long d) const;
	Rect operator-(long d) const;

	Rect& operator+=(const Point& point);
	Rect& operator-=(const Point& point);

	Rect& operator+=(long d);
	Rect& operator-=(long d);

	bool operator==(const Rect& other) const;
	bool operator!=(const Rect& other) const;

	operator long*(void) { return m; }
	operator const long*(void) const { return m; }

public:
	union 
	{
        struct 
		{
        	long left;
			long top;
			long right;
			long bottom;
        };

        long m[4];
    };
};

class RectF
{
public:
	RectF(void);
	RectF(float left,float top,float right,float bottom);
	RectF(float width,float height);
	RectF(const PointF& topLeft,const SizeF& size);
	RectF(const PointF& topLeft,const PointF& bottomRight);
	RectF(const float* m);
	RectF(const RectF& other);
	explicit RectF(const Rect& other);

	float GetWidth(void) const;
	float GetHeight(void) const;

	void SetWidth(float width);
	void SetHeight(float height);
	void SetWidthRight(float width);
	void SetHeightRight(float height);

	void Set(float left,float top,float right,float bottom);
	void Set(float width,float height);
	void Set(const PointF& topLeft,const SizeF& size);
	void Set(const PointF& topLeft,const PointF& bottomRight);
	void Set(const float* m);
	void Set(const RectF& other);
	void Set(const Rect& other);

	void SetEmpty(void);
	bool Empty(void) const;

	void AdjustWidth(void);
	void AdjustHeight(void);
	void Adjust(void);

	void Inflate(float dx,float dy);
	void Inflate(float d);

	void Offset(float dx,float dy);
	void Offset(float d);

	void Scale(float dx,float dy);
	void Scale(float d);

	bool PointIn(const PointF& point) const;

	PointF GetCenter(void) const;

	// TODO You should check if the result rect is valid if there is a chance that the two rects don't intersect!
	void Intersect(const RectF& other);

	RectF operator+(const PointF& point) const;
	RectF operator-(const PointF& point) const;

	RectF operator+(float d) const;
	RectF operator-(float d) const;

	RectF& operator+=(const PointF& point);
	RectF& operator-=(const PointF& point);

	RectF& operator+=(float d);
	RectF& operator-=(float d);

	bool operator==(const RectF& other) const;
	bool operator!=(const RectF& other) const;

	operator float*(void) { return m; }
	operator const float*(void) const { return m; }

public:
	union 
	{
        struct 
		{
        	float left;
			float top;
			float right;
			float bottom;
        };

        float m[4];
    };
};