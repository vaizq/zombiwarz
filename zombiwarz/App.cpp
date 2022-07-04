#include "App.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <cstdlib>


constexpr float PLAYER_VELO = 230.0f;
constexpr float BULLET_VELO = 1000.0f;


void App::set_player_rotation(sf::Vector2f mouse_pos)
{
	auto new_dir = mouse_pos - player.getPosition();
	float new_angle = angle_between(mouse_pos - player.getPosition(), sf::Vector2f(1, 0));
	player.setRotation(-rad2deg(new_angle) + -90);
}


App::App()
	: window(sf::VideoMode(1200, 800), "ZombieWarz", sf::Style::Fullscreen)
{
	srand(std::time(nullptr));
	window.setFramerateLimit(120);
	window.setMouseCursorVisible(true);

	kill_count_text.setFont(font);
	kill_count_text.setPosition(50, 50);
	kill_count_text.setFillColor(sf::Color(200, 0, 0));

	if (!bullet_tex.loadFromFile("bullet.png")) 
	{
		std::cerr << "Failed to load bullet texture\n";
		exit(EXIT_FAILURE);
	}

	if (!zombie_tex.loadFromFile("zombie.png"))
	{
		std::cerr << "Failed to load zombie texture\n";
		exit(EXIT_FAILURE);
	}

	if (!blood_spash_tex.loadFromFile("blood.png"))
	{
		std::cerr << "Failed to load blood texture\n";
		exit(EXIT_FAILURE);
	}
	if (!font.loadFromFile("splash.ttf"))
	{
		std::cerr << "Failed to load font\n";
		exit(EXIT_FAILURE);
	}

	if (!music.openFromFile("dark-music.wav"))
	{
		std::cerr << "Failed to load dark music\n";
		exit(EXIT_FAILURE);
	}
	music.play();
	initGame();
}

int App::run()
{
	game_state = GameState::START;
    while (game_state != GameState::QUIT)
    {
		switch (game_state)
		{
		case GameState::START:
			showStartScreen();
			initGame();
			break;
		case GameState::RUNNING:
		{
			const float dt = timer.mark();
			update(dt);
			render(1.0f / dt);
			break;
		}
		case GameState::FINISH:
			showFinishScreen();
			break;
		case GameState::QUIT:
			break;
		}
    }

	window.close();
	return 0;
}

void App::update(float dt)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			game_state = GameState::QUIT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		game_state = GameState::QUIT;
	}

	player.update();

	// Handle player movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		player.move(sf::Vector2f(0, -1) * PLAYER_VELO * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		player.move(sf::Vector2f(1, 0) * PLAYER_VELO * dt);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		player.move(sf::Vector2f(0, 1) * PLAYER_VELO * dt);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		player.move(sf::Vector2f(-1, 0) * PLAYER_VELO * dt);

	}


	// Rotate player to face mouse position
	const auto mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
	set_player_rotation(mouse_pos);


	// Shoot
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!left_pressed)
		{
			// Create bullet, set position, rotation and push to bullets container
			player.shoot();
			auto pos = player.getBarrelPos();
			auto velo = normalize(mouse_pos - pos) * BULLET_VELO * dt;
			Bullet bullet(pos, velo, bullet_tex);
			float new_angle = angle_between(mouse_pos - pos, sf::Vector2f(1, 0));
			bullet.setRotation(-rad2deg(new_angle) + 180);
			bullets.push_back(bullet);
		}
		left_pressed = true;
	}
	else {
		left_pressed = false;
	}

	// Update bullets
	for (auto& bullet : bullets)
	{
		bullet.updatePos();
	}

	// Remove bullets that are out of window
	bullets.remove_if([&](Bullet& bullet) {
		// TODO fix this
		auto bounds = bullet.getGlobalBounds();
		sf::FloatRect window_bounds(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(window.getSize()));
		return !window_bounds.intersects(bounds);
	});


	// Create zombies
	if (zombies.size() == 0)
	{
		int count = wave * wave;
		for (int i = 0; i < count; i++)
		{
			Zombie zombie(zombie_tex, blood_spash_tex);
			float angle = 2 * PI * ((std::rand() % 666) / 666.0f);
			float r = sqrt(pow(window.getSize().x / 2, 2) + pow(window.getSize().y / 2, 2));
			zombie.setPosition(window.getSize().x / 2 + r * cos(angle), window.getSize().y / 2 + r * sin(angle));
			zombies.push_back(zombie);
		}
		++wave;
	}

	// Update zombies
	for (auto& zombie : zombies)
	{
		zombie.updatePos(player.getPosition());
	}

	// Kill zombies that got hit by a bullet
	for (auto& zombie : zombies)
	{
		for (const auto& bullet : bullets)
		{
			if (bullet.getGlobalBounds().intersects(zombie.getGlobalBounds()))
			{
				if (!zombie.hasBeenShot())
				{
					zombie.gotShot();
					kills_count++;
				}
			}
		}
	}

	// Remove bullets that hit zombies
	bullets.remove_if([&](Bullet& bullet) {
		for (const auto& zombie : zombies) {
			if (bullet.getGlobalBounds().intersects(zombie.getGlobalBounds()))
				return true;
		}
		return false;
		});


	// Remove dead zombies
	zombies.remove_if([&](Zombie& zombie) {
		return zombie.isDead();
		});

	// End game if zombie kills player
	for (const auto& zombie : zombies)
	{
		if (zombie.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			player.die();
			game_state = GameState::FINISH;
		}
	}
}

void App::render(float fps)
{
	kill_count_text.setString("Kills: " + std::to_string(kills_count));
	window.clear();
	window.draw(kill_count_text);
	window.draw(kill_count_text);
	window.draw(player);
	for (const auto& zombie : zombies)
	{
		window.draw(zombie);
	}
	for (const auto& bullet : bullets)
	{
		window.draw(bullet);
	}
	window.display();
}

void App::initGame()
{
	player.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	player.setRotation(0.0f);
	bullets.clear();
	zombies.clear();
	wave = 1;
	kills_count = 0;
	timer.mark();
}

void App::showStartScreen()
{
	sf::Text logo_text;
	int size = window.getSize().y / 4;
	logo_text.setString("ZombieWarz");
	logo_text.setFont(font);
	logo_text.setFillColor(sf::Color(200, 0, 0));
	logo_text.setCharacterSize(size);
	logo_text.setPosition((window.getSize().x - logo_text.getGlobalBounds().width) / 2, window.getSize().y / 2 - size);

	sf::Text info_text;
	size /= 4;
	info_text.setString("Press any key to play");
	info_text.setFont(font);
	info_text.setFillColor(sf::Color(200, 0, 0));
	info_text.setCharacterSize(size);
	info_text.setPosition((window.getSize().x - info_text.getGlobalBounds().width) / 2, window.getSize().y / 1.3 - size);


	float scale = 1.0f;
	float ds = 1.1f;

	auto t0 = std::chrono::steady_clock::now();

	while (window.isOpen() && game_state == GameState::START)
	{
		auto t1 = std::chrono::steady_clock::now();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				game_state = GameState::QUIT;
				break;
			}
			if (event.type == sf::Event::KeyPressed && std::chrono::duration<float>(t1 -t0).count() > 1.0f) {
				game_state = GameState::RUNNING;
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			game_state = GameState::QUIT;
			break;
		}

		if (scale >= 1.1f) {
			ds = 0.999f;
		}
		if (scale <= 0.9f)
			ds = 1.001f;

		scale *= ds;
		info_text.setScale(scale, scale);

		window.clear();
		window.draw(logo_text);
		window.draw(info_text);
		window.display();
	}
}

void App::showFinishScreen()
{
	sf::Text logo_text;
	int size = window.getSize().y / 8;
	logo_text.setString("Zombies Killed: " + std::to_string(kills_count));
	logo_text.setFont(font);
	logo_text.setFillColor(sf::Color(200, 0, 0));
	logo_text.setCharacterSize(size);
	logo_text.setPosition((window.getSize().x - logo_text.getGlobalBounds().width) / 2, window.getSize().y / 2 - size);

	sf::Text info_text;
	size /= 2;
	info_text.setString("Press any key to continue");
	info_text.setFont(font);
	info_text.setFillColor(sf::Color(200, 0, 0));
	info_text.setCharacterSize(size);
	info_text.setPosition((window.getSize().x - info_text.getGlobalBounds().width) / 2, window.getSize().y / 1.3 - size);

	window.clear();
	window.draw(logo_text);
	window.draw(info_text);
	window.display();

	auto t0 = std::chrono::steady_clock::now();

	while (window.isOpen() && game_state == GameState::FINISH)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			auto t1 = std::chrono::steady_clock::now();

			if (event.type == sf::Event::Closed) {
				game_state = GameState::QUIT;
				break;
			}
			if (event.type == sf::Event::KeyPressed && std::chrono::duration<float>(t1 - t0).count() > 1.0f) {
				game_state = GameState::START;
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			game_state = GameState::QUIT;
			break;
		}

	}
}



