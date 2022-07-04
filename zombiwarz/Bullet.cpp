#include "Bullet.h"
#include <iostream>
#include "utils.h"

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f velo, const sf::Texture& tex)
{
//	constexpr float ADJUSTMENT = -PI * 0.008f;
//	this->velo = vec_rotate(velo, ADJUSTMENT);
	this->velo = velo;
	setTexture(tex);
	setPosition(pos);
	setScale(0.5, 0.5);
}

void Bullet::updatePos()
{
	move(velo);
}
