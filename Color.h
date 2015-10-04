#pragma once

class Color
{
public:
	Color(void);
	Color(float a,float r,float g,float b);
	Color(float r,float g,float b);
	Color(float a,float value);
	Color(float value);
	Color(const Color& other);
	Color(float a,const Color& other);
	Color(const float* m);

	void Set(float a,float r,float g,float b);
	void Set(float r,float g,float b);
	void Set(float a,float value);
	void Set(float value);
	void Set(const Color& other);
	void Set(float a,const Color& other);
	void Set(const float* m);
	
	void SetEmpty(void);
	bool Empty(void);

	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;
	bool operator<(const Color& other) const;
	bool operator>(const Color& other) const;
	bool operator<=(const Color& other) const;
	bool operator>=(const Color& other) const;

	void Clamp(float mn,float mx);
	bool Clamped(void) const;

	void LinearInterp(const Color& dest,float s);
	void CubicInterp(const Color& dest,float s);
	void CosInterp(const Color& dest,float s);
	
	void Brighten(float s);
	void Darken(float s);
	
	void Saturation(float s);
	void Negative(void);
	void Grayscale(void);
	void Shadow(void);

	operator unsigned long(void) const;

	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;

	Color& operator+=(const Color& other);
	Color& operator-=(const Color& other);

	Color operator*(const float scalar) const;
	Color operator/(const float scalar) const;

	Color& operator*=(const float scalar);
	Color& operator/=(const float scalar);

	Color operator*(const Color& other) const;
	Color& operator*=(const Color& other);

	operator float*(void) { return m; }
	operator const float*(void) const { return m; }

public:
	union 
	{
		struct 
		{
			float a;
			float r;
			float g;
			float b;
		};

		float m[4];
	};

public:
	static unsigned long AliceBlue;
	static unsigned long AntiqueWhite;
	static unsigned long Aqua;
	static unsigned long Aquamarine;
	static unsigned long Azure;
	static unsigned long Beige;
	static unsigned long Bisque;
	static unsigned long Black;
	static unsigned long BlanchedAlmond;
	static unsigned long Blue;
	static unsigned long BlueViolet;
	static unsigned long Brown;
	static unsigned long BurlyWood;
	static unsigned long CadetBlue;
	static unsigned long Chartreuse;
	static unsigned long Chocolate;
	static unsigned long Coral;
	static unsigned long CornflowerBlue;
	static unsigned long Cornsilk;
	static unsigned long Crimson;
	static unsigned long Cyan;
	static unsigned long DarkBlue;
	static unsigned long DarkCyan;
	static unsigned long DarkGoldenRod;
	static unsigned long DarkGray;
	static unsigned long DarkGreen;
	static unsigned long DarkKhaki;
	static unsigned long DarkMagenta;
	static unsigned long DarkOliveGreen;
	static unsigned long Darkorange;
	static unsigned long DarkOrchid;
	static unsigned long DarkRed;
	static unsigned long DarkSalmon;
	static unsigned long DarkSeaGreen;
	static unsigned long DarkSlateBlue;
	static unsigned long DarkSlateGray;
	static unsigned long DarkTurquoise;
	static unsigned long DarkViolet;
	static unsigned long DeepPink;
	static unsigned long DeepSkyBlue;
	static unsigned long DimGray;
	static unsigned long DodgerBlue;
	static unsigned long Feldspar;
	static unsigned long FireBrick;
	static unsigned long FloralWhite;
	static unsigned long ForestGreen;
	static unsigned long Fuchsia;
	static unsigned long Gainsboro;
	static unsigned long GhostWhite;
	static unsigned long Gold;
	static unsigned long GoldenRod;
	static unsigned long Gray;
	static unsigned long Green;
	static unsigned long GreenYellow;
	static unsigned long HoneyDew;
	static unsigned long HotPink;
	static unsigned long IndianRed ;
	static unsigned long Indigo ;
	static unsigned long Ivory;
	static unsigned long Khaki;
	static unsigned long Lavender;
	static unsigned long LavenderBlush;
	static unsigned long LawnGreen;
	static unsigned long LemonChiffon;
	static unsigned long LightBlue;
	static unsigned long LightCoral;
	static unsigned long LightCyan;
	static unsigned long LightGoldenRodYellow;
	static unsigned long LightGrey;
	static unsigned long LightGreen;
	static unsigned long LightPink;
	static unsigned long LightSalmon;
	static unsigned long LightSeaGreen;
	static unsigned long LightSkyBlue;
	static unsigned long LightSlateBlue;
	static unsigned long LightSlateGray;
	static unsigned long LightSteelBlue;
	static unsigned long LightYellow;
	static unsigned long Lime;
	static unsigned long LimeGreen;
	static unsigned long Linen;
	static unsigned long Magenta;
	static unsigned long Maroon;
	static unsigned long MediumAquaMarine;
	static unsigned long MediumBlue;
	static unsigned long MediumOrchid;
	static unsigned long MediumPurple;
	static unsigned long MediumSeaGreen;
	static unsigned long MediumSlateBlue;
	static unsigned long MediumSpringGreen;
	static unsigned long MediumTurquoise;
	static unsigned long MediumVioletRed;
	static unsigned long MidnightBlue;
	static unsigned long MintCream;
	static unsigned long MistyRose;
	static unsigned long Moccasin;
	static unsigned long NavajoWhite;
	static unsigned long Navy;
	static unsigned long OldLace;
	static unsigned long Olive;
	static unsigned long OliveDrab;
	static unsigned long Orange;
	static unsigned long OrangeRed;
	static unsigned long Orchid;
	static unsigned long PaleGoldenRod;
	static unsigned long PaleGreen;
	static unsigned long PaleTurquoise;
	static unsigned long PaleVioletRed;
	static unsigned long PapayaWhip;
	static unsigned long PeachPuff;
	static unsigned long Peru;
	static unsigned long Pink;
	static unsigned long Plum;
	static unsigned long PowderBlue;
	static unsigned long Purple;
	static unsigned long Red;
	static unsigned long RosyBrown;
	static unsigned long RoyalBlue;
	static unsigned long SaddleBrown;
	static unsigned long Salmon;
	static unsigned long SandyBrown;
	static unsigned long SeaGreen;
	static unsigned long SeaShell;
	static unsigned long Sienna;
	static unsigned long Silver;
	static unsigned long SkyBlue;
	static unsigned long SlateBlue;
	static unsigned long SlateGray;
	static unsigned long Snow;
	static unsigned long SpringGreen;
	static unsigned long SteelBlue;
	static unsigned long Tan;
	static unsigned long Teal;
	static unsigned long Thistle;
	static unsigned long Tomato;
	static unsigned long Turquoise;
	static unsigned long Violet;
	static unsigned long VioletRed;
	static unsigned long Wheat;
	static unsigned long White;
	static unsigned long WhiteSmoke;
	static unsigned long Yellow;
	static unsigned long YellowGreen;
};