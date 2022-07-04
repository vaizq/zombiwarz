#pragma once
#include <SFML/Graphics.hpp>


class Bullet : public sf::Sprite
{
public:
	Bullet(sf::Vector2f pos, sf::Vector2f velo, const sf::Texture& tex);
	void updatePos();
private:
	sf::Vector2f velo;
};
