#pragma once
#include <SFML/Graphics.hpp>

constexpr float PI = 3.14159265;

float deg2rad(float x);
float rad2deg(float x);

sf::Vector2f normalize(sf::Vector2f);
float dotprod(sf::Vector2f a, sf::Vector2f b);
float angle_between(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f vec_rotate(sf::Vector2f a, float angle);
