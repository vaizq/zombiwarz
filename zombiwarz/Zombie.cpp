#include "Zombie.h"
#include "utils.h"

constexpr float ZOMBIE_VELO = 80.0f;
constexpr float DIE_TIME = 1.0f;

Zombie::Zombie(const sf::Texture& tex, const sf::Texture& blood_tex)
	: blood_tex {blood_tex}
{
	setTexture(tex, true);
	setScale(0.5, 0.5);

	blood_splash.setTexture(blood_tex);
	blood_splash.scale(0.25, 0.25);
	blood_splash.setOrigin(blood_splash.getGlobalBounds().width / 2, blood_splash.getGlobalBounds().height / 2);
	timer.mark();
}

void Zombie::updatePos(sf::Vector2f player_pos)
{
	float dt = timer.mark();

	sf::Vector2f velo = normalize(player_pos - getPosition()) * ZOMBIE_VELO * dt;
	float angle = angle_between(velo, sf::Vector2f(1, 0));
	setRotation(rad2deg(-angle) + 90.0f);
	if (got_shot)
	{
		// Slow down if got shot
		auto t_now = std::chrono::steady_clock::now();
		float duration = std::chrono::duration<float>(t_now - shot_time).count();
		velo *= (DIE_TIME - duration) / DIE_TIME;
	}
	move(velo);

	blood_splash.setPosition(getPosition());
	blood_splash.setRotation(getRotation());
}

void Zombie::gotShot()
{
	got_shot = true;
	shot_time = std::chrono::steady_clock::now();
}

bool Zombie::hasBeenShot() const
{
	return got_shot;
}

bool Zombie::isDead() const
{
	auto t_now = std::chrono::steady_clock::now();
	return got_shot && std::chrono::duration<float>(t_now - shot_time).count() > DIE_TIME;
}

void Zombie::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(static_cast<sf::Sprite>(*this), states);
	if (got_shot)
	{
		target.draw(blood_splash, states);
	}
}
