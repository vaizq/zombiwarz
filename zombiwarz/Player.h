#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FrameTimer.h"
#include <chrono>


class Player : public sf::Sprite
{
public:
	Player(sf::Vector2f pos = sf::Vector2f(0,0));
	sf::Vector2f getBarrelPos() const;
	void shoot();
	void die();
	void update(); // Call every frame
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Texture tex;
	sf::Sprite gunfire;
	sf::Texture gunfire_tex;
	bool shooting = false;
	std::chrono::time_point<std::chrono::steady_clock> shot_time;
	sf::SoundBuffer gunsound;
	sf::SoundBuffer deathsound;
	sf::Sound sound;
};

