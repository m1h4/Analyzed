#pragma once

class Point;
class PointF;

class Point
{
public:
	Point(void);
	Point(long x,long y);
	Point(long xy);
	Point(const long* m);
	Point(const Point& other);
	explicit Point(const PointF& other);

	void Set(long x,long y);
	void Set(long xy);
	void Set(const long* m);
	void Set(const Point& other);
	void Set(const PointF& other);

	void SetEmpty(void);
	bool Empty(void) const;

	void Offset(long dx,long dy);
	void Offset(long d);

	bool operator==(const Point& other) const;
	bool operator!=(const Point& other) const;

	Point operator+(const Point& other) const;
	Point operator-(const Point& other) const;

	Point operator+(long d) const;
	Point operator-(long d) const;

	Point& operator+=(const Point& other);
	Point& operator-=(const Point& other);

	Point& operator+=(long d);
	Point& operator-=(long d);

	operator long*(void) { return m; }
	operator const long*(void) const { return m; }

public:
	union 
	{
        struct 
		{
        	long x;
			long y;
        };

        long m[2];
    };
};

class PointF
{
public:
	PointF(void);
	PointF(float x,float y);
	PointF(float xy);
	PointF(const float* m);
	PointF(const PointF& other);
	explicit PointF(const Point& other);

	void Set(float x,float y);
	void Set(float xy);
	void Set(const float* m);
	void Set(const PointF& other);
	void Set(const Point& other);

	void SetEmpty(void);
	bool Empty(void) const;

	void Offset(float dx,float dy);
	void Offset(float d);

	bool operator==(const PointF& other) const;
	bool operator!=(const PointF& other) const;

	PointF operator+(const PointF& other) const;
	PointF operator-(const PointF& other) const;

	PointF operator+(float d) const;
	PointF operator-(float d) const;

	PointF& operator+=(const PointF& other);
	PointF& operator-=(const PointF& other);

	PointF& operator+=(float d);
	PointF& operator-=(float d);

	operator float*(void) { return m; }
	operator const float*(void) const { return m; }

public:
	union 
	{
        struct 
		{
        	float x;
			float y;
        };

        float m[2];
    };
};