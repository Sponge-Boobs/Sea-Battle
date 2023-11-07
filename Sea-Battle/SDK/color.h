#pragma once
#include "imgui/imgui.h"

class Color
{
public:
	// constructors
	constexpr Color() = default;
	Color(int color32)
	{
		_full = color32;
	}
	Color(unsigned int color32)
	{
		_full = color32;
	}
	constexpr Color(int _r, int _g, int _b): _color{static_cast<unsigned char>(_r), static_cast<unsigned char>(_g), static_cast<unsigned char>(_b), 255}
	{}
	constexpr Color(int _r, int _g, int _b, int _a): _color { static_cast<unsigned char>(_r), static_cast<unsigned char>(_g), static_cast<unsigned char>(_b), static_cast<unsigned char>(_a) }
	{}
	explicit Color(float color[])
	{
		SetColor(static_cast<int>(color[0]), static_cast<int>(color[1]), static_cast<int>(color[2]), static_cast<int>(color[3]));
	}
	void SetColor(int _r, int _g, int _b, int _a = 255)
	{
		_color[0] = static_cast<unsigned char>(_r);
		_color[1] = static_cast<unsigned char>(_g);
		_color[2] = static_cast<unsigned char>(_b);
		_color[3] = static_cast<unsigned char>(_a);
	}
	void GetColor(int& _r, int& _g, int& _b, int& _a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}
	void SetRawColor(int color32)
	{
		*reinterpret_cast<int*>(this) = color32;
	}
	void SetAlpha(int alpha)
	{
		_color[3] = static_cast<unsigned char>(alpha);
	}
	int GetRawColor()
	{
		return *reinterpret_cast<int*>(this);
	}
	int r() const
	{
		return _color[0];
	}
	int g() const
	{
		return _color[1];
	}
	int b() const
	{
		return _color[2];
	}
	int a() const
	{
		return _color[3];
	}
	unsigned char& operator[](int index)
	{
		return _color[index];
	}
	const unsigned char& operator[](int index) const
	{
		return _color[index];
	}
	bool operator == (const Color& rhs) const
	{
		return *(int*)this == *(int*)&rhs;
	}
	bool operator != (const Color& rhs) const
	{
		return !(operator==(rhs));
	}
	operator ImU32() const
	{
		return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8) + (_color[0] & 0xff);
	}

	static Color FromHSB(float hue, float saturation, float brightness)
	{
		auto h = hue == 1.0f ? 0 : hue * 6.0f;
		auto f = h - static_cast<int>(h);
		auto p = brightness * (1.0f - saturation);
		auto q = brightness * (1.0f - saturation * f);
		auto t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				static_cast<unsigned char>(brightness * 255),
				static_cast<unsigned char>(t * 255),
				static_cast<unsigned char>(p * 255)
			);
		}
		if (h < 2)
		{
			return Color(
				static_cast<unsigned char>(q * 255),
				static_cast<unsigned char>(brightness * 255),
				static_cast<unsigned char>(p * 255)
			);
		}
		if (h < 3)
		{
			return Color(
				static_cast<unsigned char>(p * 255),
				static_cast<unsigned char>(brightness * 255),
				static_cast<unsigned char>(t * 255)
			);
		}
		if (h < 4)
		{
			return Color(
				static_cast<unsigned char>(p * 255),
				static_cast<unsigned char>(q * 255),
				static_cast<unsigned char>(brightness * 255)
			);
		}
		if (h < 5)
		{
			return Color(
				static_cast<unsigned char>(t * 255),
				static_cast<unsigned char>(p * 255),
				static_cast<unsigned char>(brightness * 255)
			);
		}
		return Color(
			static_cast<unsigned char>(brightness * 255),
			static_cast<unsigned char>(p * 255),
			static_cast<unsigned char>(q * 255)
		);
	}

	static constexpr Color BlackTrans()
	{
		constexpr Color col(0, 0, 0, 0);
		return col;
	}
	static constexpr Color Black()
	{
		constexpr Color col(0, 0, 0, 255);;
		return col;
	}
	static constexpr Color White()
	{
		constexpr Color col(255, 255, 255, 255);
		return col;
	}
	static constexpr Color Red()
	{
		constexpr Color col(255, 0, 0, 255);
		return col;
	}
	static constexpr Color Green()
	{
		constexpr Color col(0, 128, 0, 255);
		return col;
	}
	static constexpr Color Blue()
	{
		constexpr Color col(0, 0, 255, 255);
		return col;
	}
	static constexpr Color Lime()
	{
		constexpr Color col(0, 255, 0, 255);
		return col;
	}
	static constexpr Color Yellow()
	{
		constexpr Color col(255, 255, 0, 255);
		return col;
	}
	static constexpr Color Cyan()
	{
		constexpr Color col(0, 255, 255, 255);
		return col;
	}
	static constexpr Color Magenta()
	{
		constexpr Color col(255, 0, 255, 255);
		return col;
	}
	static constexpr Color Silver()
	{
		constexpr Color col(192, 192, 192, 255);
		return col;
	}
	static constexpr Color Gray()
	{
		constexpr Color col(128, 128, 128, 255);
		return col;
	}
	static constexpr Color Maroon()
	{
		constexpr Color col(128, 0, 0, 255);
		return col;
	}
	static constexpr Color Olive()
	{
		constexpr Color col(128, 128, 0, 255);
		return col;
	}
	static constexpr Color Purple()
	{
		constexpr Color col(128, 0, 128, 255);
		return col;
	}
	static constexpr Color Teal()
	{
		constexpr Color col(0, 128, 128, 255);
		return col;
	}
	static constexpr Color Navy()
	{
		constexpr Color col(0, 0, 128, 255);
		return col;
	}
	static constexpr Color DarkRed()
	{
		constexpr Color col(139, 0, 0, 255);
		return col;
	}
	static constexpr Color Brown()
	{
		constexpr Color col(165, 42, 42, 255);
		return col;
	}
	static constexpr Color Firebrick()
	{
		constexpr Color col(178, 34, 34, 255);
		return col;
	}
	static constexpr Color Crimson()
	{
		constexpr Color col(220, 20, 60, 255);
		return col;
	}
	static constexpr Color IndianRed()
	{
		constexpr Color col(205, 92, 92, 255);
		return col;
	}
	static constexpr Color LightCoral()
	{
		constexpr Color col(240, 128, 128, 255);
		return col;
	}
	static constexpr Color DarkSalmon()
	{
		constexpr Color col(233, 150, 122, 255);
		return col;
	}
	static constexpr Color Salmon()
	{
		constexpr Color col(250, 128, 114, 255);
		return col;
	}
	static constexpr Color LightSalmon()
	{
		constexpr Color col(255, 160, 122, 255);
		return col;
	}
	static constexpr Color OrangeRed()
	{
		constexpr Color col(255, 69, 0, 255);
		return col;
	}
	static constexpr Color DarkOrange()
	{
		constexpr Color col(255, 140, 0, 255);
		return col;
	}
	static constexpr Color Orange()
	{
		constexpr Color col(255, 165, 0, 255);
		return col;
	}
	static constexpr Color OrangeR8()
	{
		constexpr Color col(255, 128, 0, 255);
		return col;
	}
	static constexpr Color Gold()
	{
		constexpr Color col(255, 215, 0, 255);
		return col;
	}
	static constexpr Color DarkGoldenRod()
	{
		constexpr Color col(184, 134, 11, 255);
		return col;
	}
	static constexpr Color GoldenRod()
	{
		constexpr Color col(218, 165, 32, 255);
		return col;
	}
	static constexpr Color YellowGreen()
	{
		constexpr Color col(154, 205, 50, 255);
		return col;
	}
	static constexpr Color DarkOliveGreen()
	{
		constexpr Color col(85, 107, 47, 255);
		return col;
	}

private:
	union
	{
		unsigned char _color[4]{};
		unsigned int _full;
	};
};