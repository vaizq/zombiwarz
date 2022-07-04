#include "Player.h"
#include <iostream>
#include "utils.h"

Player::Player(sf::Vector2f pos)
{
    if (!tex.loadFromFile("topdownshooter.png"))
    {
        std::cout << "Failed to load topdownshooter.png" << std::endl;
        exit(EXIT_FAILURE);
    }
    setTexture(tex);
    scale(0.5, 0.5);
	setOrigin(140, 80);
    setPosition(pos);


	if (!gunfire_tex.loadFromFile("gunfire.png"))
	{
		std::cerr << "Failed to load gunifire texture\n";
		exit(EXIT_FAILURE);
	}
	gunfire.setTexture(gunfire_tex);
    gunfire.scale(0.3, 0.3);
    gunfire.setOrigin(40, 35);

    if (!gunsound.loadFromFile("gunshot.wav"))
    {
        std::cerr << "Failed to load gunshot sound\n";
        exit(EXIT_FAILURE);
    }
    if (!deathsound.loadFromFile("screaming.wav"))
    {
        std::cerr << "Failed to load deathsound\n";
        exit(EXIT_FAILURE);
    }
}

sf::Vector2f Player::getBarrelPos() const
{
    sf::Vector2f pos = getPosition();

    float b = deg2rad(getRotation());
    float a = -(b <= PI ? PI - b : (3 * PI - b)) + PI * 0.025;
    sf::Vector2f adjustment(cos(a), sin(a));
    return pos + adjustment * 80.0f;
}

void Player::shoot()
{
    shooting = true;
    shot_time = std::chrono::steady_clock::now();
    sound.setBuffer(gunsound);
    sound.play();
}

void Player::die()
{
    sound.setBuffer(deathsound);
    sound.play();
}

void Player::update()
{
    gunfire.setPosition(getBarrelPos());
    gunfire.setRotation(getRotation() + 180);

    auto t_now = std::chrono::steady_clock::now();
    if (std::chrono::duration<float>(t_now - shot_time).count() > 0.2f)
    {
        shooting = false;
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(static_cast<sf::Sprite>(*this), states);
    if (shooting)
    {
        target.draw(gunfire, states);
    }
}



