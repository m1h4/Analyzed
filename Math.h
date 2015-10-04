#pragma once

static const float PI = (float)3.141592654;
static const float  PI2 = (float)6.283185308;

class Math
{
public:
	static int Rand(int min,int max);
	static float Rand(float min,float max);
	static double Rand(double min,double max);

	static float Sin(float angle);
	static double Sin(double angle);

	static float Cos(float angle);
	static double Cos(double angle);

	static float Tan(float angle);
	static double Tan(double angle);

	static float Pow(float b,float e);
	static double Pow(double b,double e);

	static float Sqrt(float v);
	static double Sqrt(double v);

	static int Abs(int p1);
	static float Abs(float p1);
	static double Abs(double p1);

	static int Max(int p1,int p2);
	static unsigned int Max(unsigned int p1,unsigned int p2);
	static float Max(float p1,float p2);
	static double Max(double p1,double p2);

	static int Min(int p1,int p2);
	static unsigned int Min(unsigned int p1,unsigned int p2);
	static float Min(float p1,float p2);
	static double Min(double p1,double p2);

	static float Lerp(float p1,float p2,float s);
	static double Lerp(double p1,double p2,double s);

	static float Cubic(float p1,float p2,float s);
	static double Cubic(double p1,double p2,double s);

	static float Sin(float p1,float p2,float s);
	static double Sin(double p1,double p2,double s);

	static float Transition(float p1,float p2,float s);
	static double Transition(double p1,double p2,double s);

	static float CatmullRom(float p1,float p2,float p3,float p4,float s);
	static double CatmullRom(double p1,double p2,double p3,double p4,double s);

	static float Hermitte(float p1,float p2,float p3,float p4,float s);
	static double Hermitte(double p1,double p2,double p3,double p4,double s);

	static int Wrap(int p,int step,int limit1,int limit2);
	static float Wrap(float p,float step,float limit1,float limit2);
	static double Wrap(double p,double step,double limit1,double limit2);

	static int Clamp(int p,int step,int limit1,int limit2);
	static float Clamp(float p,float step,float limit1,float limit2);
	static double Clamp(double p,double step,double limit1,double limit2);
};