#include <iostream>

class Vec2
{
public:
	float x;
	float y;

	Vec2();

	Vec2(float xin, float yin);

	bool operator == (const Vec2& Rhs) const;
	bool operator != (const Vec2& Rhs) const;

	
	Vec2 operator + (const Vec2& Rhs) const;
	Vec2 operator - (const Vec2& Rhs) const;
	Vec2 operator / (const float val) const;
	Vec2 operator * (const float val) const;

	void operator += (const Vec2& Rhs);
	void operator -= (const Vec2& Rhs);
	void operator *= (const Vec2& Rhs);
	void operator /= (const Vec2& Rhs);


	float dist(Vec2& v) const; //distance between two points
	void normalize();
	Vec2 getposition();
};
