#include "Globals.h"
#include "Math.h"

#include <math.h>

int Math::Rand(int min,int max)
{
	return min + (rand() % max - min);
}

float Math::Rand(float min,float max)
{
	return min + (max - min) * ((float)rand() / RAND_MAX);
}

double Math::Rand(double min,double max)
{
	return min + (max - min) * ((double)rand() / RAND_MAX);
}

float Math::Sin(float angle)
{
	return sinf(angle);
}

double Math::Sin(double angle)
{
	return sin(angle);
}

float Math::Cos(float angle)
{
	return cosf(angle);
}

double Math::Cos(double angle)
{
	return cos(angle);
}

float Math::Tan(float angle)
{
	return tanf(angle);
}

double Math::Tan(double angle)
{
	return tan(angle);
}

float Math::Pow(float b,float e)
{
	return powf(b,e);
}

double Math::Pow(double b,double e)
{
	return pow(b,e);
}

float Math::Sqrt(float p)
{
	return sqrtf(p);
}

double Math::Sqrt(double p)
{
	return sqrt(p);
}

int Math::Abs(int p)
{
	return p < 0 ? -p : p;
}

float Math::Abs(float p)
{
	return p < 0 ? -p : p;
}

double Math::Abs(double p)
{
	return p < 0 ? -p : p;
}

int Math::Max(int p1,int p2)
{
	return p1 > p2 ? p1 : p2;
}

unsigned int Math::Max(unsigned int p1,unsigned int p2)
{
	return p1 > p2 ? p1 : p2;
}

float Math::Max(float p1,float p2)
{
	return p1 > p2 ? p1 : p2;
}

double Math::Max(double p1,double p2)
{
	return p1 > p2 ? p1 : p2;
}

int Math::Min(int p1,int p2)
{
	return p1 < p2 ? p1 : p2;
}

unsigned int Math::Min(unsigned int p1,unsigned int p2)
{
	return p1 < p2 ? p1 : p2;
}

float Math::Min(float p1,float p2)
{
	return p1 < p2 ? p1 : p2;
}

double Math::Min(double p1,double p2)
{
	return p1 < p2 ? p1 : p2;
}

float Math::Lerp(float p1,float p2,float s)
{
	return p1 + s * (p2 - p1);
}

double Math::Lerp(double p1,double p2,double s)
{
	return p1 + s * (p2 - p1);
}

float Math::Cubic(float p1,float p2,float s)
{
	float s1 = 2.0f * powf(s,3.0f);
	float s2 = 3.0f * powf(s,2.0f);

	return p1 * (s1 - s2 + 1.0f) + p2 * (s2 - s1);
}

double Math::Cubic(double p1,double p2,double s)
{
	double s1 = 2.0 * pow(s,3.0);
	double s2 = 3.0 * pow(s,2.0);

	return p1 * (s1 - s2 + 1.0) + p2 * (s2 - s1);
}

float Math::Sin(float p1,float p2,float s)
{
	return p1 + sinf(PI/2.0f * s) * (p2 - p1);
}

double Math::Sin(double p1,double p2,double s)
{
	return p1 + sin(PI/2.0 * s) * (p2 - p1);
}

float Math::Transition(float p1,float p2,float s)
{
	if(p1 < p2)
	{
		p1 += s;

		if(p1 > p2)
			p1 = p2;
	}
	else if(p1 > p2)
	{
		p1 -= s;

		if(p1 < p2)
			p1 = p2;
	}

	return p1;
}

double Math::Transition(double p1,double p2,double s)
{
	if(p1 < p2)
	{
		p1 += s;

		if(p1 > p2)
			p1 = p2;
	}
	else if(p1 > p2)
	{
		p1 -= s;

		if(p1 < p2)
			p1 = p2;
	}

	return p1;
}

float Math::CatmullRom(float p1,float p2,float p3,float p4,float s)
{
	float s2 = powf(s,2.0f);
	float s3 = powf(s,3.0f);

	return (p1 * (-s3 + 2.0f * s2 - s) + p2 * (3.0f * s3 - 5.0f * s2 + 2.0f) + p3 * (-3.0f * s3 + 4.0f * s2 + s) + p4 * (s3 - s2)) / 2.0f;
}

double Math::CatmullRom(double p1,double p2,double p3,double p4,double s)
{
	double s2 = pow(s,2.0);
	double s3 = pow(s,3.0);

	return (p1 * (-s3 + 2.0 * s2 - s) + p2 * (3.0 * s3 - 5.0 * s2 + 2.0) + p3 * (-3.0 * s3 + 4.0 * s2 + s) + p4 * (s3 - s2)) / 2.0;
}

float Math::Hermitte(float p1,float p2,float p3,float p4,float s)
{
	float t2 = s * s;
	float t3 = t2 * s;

	float kp0 = (2.0f * t2 - t3 - s) / 2.0f;
	float kp1 = (3.0f * t3 - 5.0f * t2 + 2.0f) / 2.0f;
	float kp2 = (4.0f * t2 - 3.0f * t3 + s) / 2.0f;
	float kp3 = (t3 - t2) / 2.0f;

	return p1 * kp0 + p2 * kp1 + p3 * kp2 + p4 * kp3;
}

double Math::Hermitte(double p1,double p2,double p3,double p4,double s)
{
	double t2 = s * s;
	double t3 = t2 * s;

	double kp0 = (2.0 * t2 - t3 - s) / 2.0;
	double kp1 = (3.0 * t3 - 5.0 * t2 + 2.0) / 2.0;
	double kp2 = (4.0 * t2 - 3.0 * t3 + s) / 2.0;
	double kp3 = (t3 - t2) / 2.0;

	return p1 * kp0 + p2 * kp1 + p3 * kp2 + p4 * kp3;
}

int Math::Wrap(int p,int step,int limit1,int limit2)
{
	p += step;

	if(p < limit1)
		p = limit2+1 - abs(p - limit1);
	else if(p > limit2)
		p = limit1 + abs(p - (limit2+1));

	return p;
}

float Math::Wrap(float p,float step,float limit1,float limit2)
{
	p += step;

	if(p < limit1)
		p = limit2+1.0f - abs(p - limit1);
	else if(p > limit2)
		p = limit1 + abs(p - (limit2+1.0f));

	return p;
}

double Math::Wrap(double p,double step,double limit1,double limit2)
{
	p += step;

	if(p < limit1)
		p = limit2+1.0 - abs(p - limit1);
	else if(p > limit2)
		p = limit1 + abs(p - (limit2+1.0));

	return p;
}

int Math::Clamp(int p,int step,int limit1,int limit2)
{
	p += step;

	if(p < limit1)
		p = limit1;
	else if(p > limit2)
		p = limit2;

	return p;
}

float Math::Clamp(float p,float step,float limit1,float limit2)
{
	p += step;

	if(p < limit1)
		p = limit1;
	else if(p > limit2)
		p = limit2;

	return p;
}

double Math::Clamp(double p,double step,double limit1,double limit2)
{
	p += step;

	if(p < limit1)
		p = limit1;
	else if(p > limit2)
		p = limit2;

	return p;
}
