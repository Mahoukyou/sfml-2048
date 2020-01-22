#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Game
{
public:
	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) noexcept = delete;

	static Game& instance();

	[[nodiscard]] const sf::RenderWindow& window() const noexcept;

	void main_loop();

private:
	Game();

	void internal_main_loop();

	std::unique_ptr<sf::RenderWindow> window_;
};