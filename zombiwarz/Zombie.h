#pragma once
#include <SFML/Graphics.hpp>
#include "FrameTimer.h"
#include <chrono>

class Zombie : public sf::Sprite
{
public:
	Zombie(const sf::Texture& tex, const sf::Texture& blood_tex);
	void updatePos(sf::Vector2f player_pos);
	void gotShot();
	bool hasBeenShot() const;
	bool isDead() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	FrameTimer timer;
	sf::Sprite blood_splash;
	const sf::Texture& blood_tex;
	std::chrono::time_point<std::chrono::steady_clock> shot_time;
	bool got_shot = false;

};


