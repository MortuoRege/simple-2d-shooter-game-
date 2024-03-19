#pragma once
//header class for all the components
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public: 
	Vec2 pos		= { 0.0, 0.0 };
	Vec2 velocity 	= { 0.0, 0.0 };
	float angle		= 0;

	CTransform(const Vec2 & p, const Vec2 & v, float a) :
		pos(p), velocity(v), angle(a){}

};
//Component for the shape of the entity
class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness) :
		circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};
//Component for the collision of the entity
class CCollision
{
public:
	float radius = 0;
	CCollision(float r):radius(r){}
};

//Component for the score of the entity
class CScore
{
public:
	int score = 0;
	CScore(int s) : score(s) {};
};
//Component for the lifespan of the entity
class CLifeSpan
{
public:
	int remaining	= 0;
	int total		= 0;
	CLifeSpan(int total ): 
		remaining(total), total(total) {}
};

//Component for the input of the entity
class CInput
{
public:
	bool up		= false;
	bool left	= false;
	bool right	= false;
	bool down	= false;
	bool shoot	= false;

	CInput() {}
};