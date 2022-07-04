#include "utils.h"

float deg2rad(float deg)
{
	return (float) (PI / 180.0 * deg);
}
float rad2deg(float rad)
{
	return (float) (180.0 / PI * rad);
}

float dotprod(sf::Vector2f a, sf::Vector2f b)
{
	return (float) (a.x * b.x + a.y * b.y);
}

float angle_between(sf::Vector2f a, sf::Vector2f b)
{
	// Normalize vectors
	a /= static_cast<float>(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	b /= static_cast<float>(sqrt(pow(b.x, 2) + pow(b.y, 2)));

	float dot = a.x * b.x + a.y * b.y;
	float det = a.x * b.y + a.y * b.x;

	return (float) atan2(dot, det);
}

sf::Vector2f vec_rotate(sf::Vector2f a, float angle)
{
	return sf::Vector2f(a.x * cos(angle) - a.y * sin(angle), a.x * sin(angle) + a.y * cos(angle));
}

sf::Vector2f normalize(sf::Vector2f v)
{
	float len = (float) sqrt(v.x * v.x + v.y * v.y);
	return v / len;
}

