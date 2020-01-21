#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Game
{
public:
	Game();
	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) noexcept = delete;

	void main_loop();

private:
	std::unique_ptr<sf::RenderWindow> window_;
};