#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "FrameTimer.h"
#include "Bullet.h"
#include <list>
#include "Zombie.h"

enum class GameState
{
	START,
	RUNNING,
	FINISH,
	QUIT
};

class App
{
public:
	App();
	int run();
private:
	void update(float dt);
	void render(float fps);
	void initGame();
	void showStartScreen();
	void showFinishScreen();
	void set_player_rotation(sf::Vector2f mouse_pos);
private:
	FrameTimer timer;
	sf::RenderWindow window;
	Player player;
	std::list<Bullet> bullets;
	sf::Texture bullet_tex;
	std::list<Zombie> zombies;
	sf::Texture zombie_tex;
	sf::Texture blood_spash_tex;
	sf::Font font;
	sf::Text kill_count_text;
	GameState game_state = GameState::START;
	sf::Music music;
	int wave = 1;
	bool left_pressed = false;
	int kills_count = 0;
};


