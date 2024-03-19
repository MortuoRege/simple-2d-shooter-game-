#include "Vec2.h"
#include <math.h>

//cpp file to impleent all the basic vector operations
//easy to use and understand if you look at the header file

Vec2::Vec2()
{

}
Vec2::Vec2(float xin, float yin) : x(xin), y(yin)
{

}
Vec2 Vec2::operator + (const Vec2& Rhs) const 
{
	return Vec2(x + Rhs.x, y + Rhs.y);
}

Vec2 Vec2::operator - (const Vec2& Rhs) const
{
	return Vec2(x - Rhs.x, y - Rhs.y);
}
Vec2 Vec2::operator / (const float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const float val) const
{
	return Vec2(x * val, y * val);
}

bool Vec2::operator == (const Vec2& Rhs) const
{
	return (x == Rhs.x && y == Rhs.y);
}

bool Vec2::operator != (const Vec2& Rhs) const
{
	return (x != Rhs.x || y != Rhs.y);
}

void Vec2::operator += (const Vec2& Rhs)
{
	x += Rhs.x;
	y += Rhs.y;
}

void Vec2::operator -= (const Vec2& Rhs)
{
	x -= Rhs.x;
	y -= Rhs.y;
}

void Vec2::operator *= (const Vec2& Rhs)
{
	x *= Rhs.x;
	y *= Rhs.y;
}

void Vec2::operator /= (const Vec2& Rhs)
{
	x /= Rhs.x;
	y /= Rhs.y;
}

float Vec2::dist(Vec2& v) const
{
	return std::sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y));
}

float Vec2::normalize() 
{
	float length = std::sqrt(x * x + y *y);
	if (length != 0)
	{
		x /= length;
		y /= length;
	}
	return length;
}

Vec2 Vec2::getposition()
{
	return Vec2(x, y);
}


