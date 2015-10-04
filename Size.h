#pragma once

class Size;
class SizeF;

class Size
{
public:
	Size(void);
	Size(long cx,long cy);
	Size(long c);
	Size(const long* m);
	Size(const Size& other);
	explicit Size(const SizeF& other);

	void Set(long cx,long cy);
	void Set(long c);
	void Set(const long* m);
	void Set(const Size& other);
	void Set(const SizeF& other);

	void SetEmpty(void);
	bool Empty(void) const;

	void Expand(long dx,long dy);
	void Expand(long d);

	bool operator==(const Size& other) const;
	bool operator!=(const Size& other) const;

	Size operator+(const Size& other) const;
	Size operator-(const Size& other) const;

	Size operator+(long d) const;
	Size operator-(long d) const;

	Size& operator+=(const Size& other);
	Size& operator-=(const Size& other);

	Size& operator+=(long d);
	Size& operator-=(long d);

	operator long*(void) { return m; }
	operator const long*(void) const { return m; }

public:
	union 
	{
        struct 
		{
        	long cx;
			long cy;
        };

        long m[2];
    };
};

class SizeF
{
public:
	SizeF(void);
	SizeF(float cx,float cy);
	SizeF(float c);
	SizeF(const float* m);
	SizeF(const SizeF& other);
	explicit SizeF(const Size& other);

	void Set(float cx,float cy);
	void Set(float c);
	void Set(const float* m);
	void Set(const SizeF& other);
	void Set(const Size& other);

	void SetEmpty(void);
	bool Empty(void) const;

	void Expand(float dx,float dy);
	void Expand(float d);

	bool operator==(const SizeF& other) const;
	bool operator!=(const SizeF& other) const;

	SizeF operator+(const SizeF& other) const;
	SizeF operator-(const SizeF& other) const;

	SizeF operator+(float d) const;
	SizeF operator-(float d) const;

	SizeF& operator+=(const SizeF& other);
	SizeF& operator-=(const SizeF& other);

	SizeF& operator+=(float d);
	SizeF& operator-=(float d);

	operator float*(void) { return m; }
	operator const float*(void) const { return m; }

public:
	union 
	{
        struct 
		{
        	float cx;
			float cy;
        };

        float m[2];
    };
};