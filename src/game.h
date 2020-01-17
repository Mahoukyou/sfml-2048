#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

class Game
{
public:
	Game();
	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) = delete;
	
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

	void main_loop();

	void set_new_scene(std::unique_ptr<IScene> new_scene);
	[[nodiscard]] const IScene* current_scene() const noexcept;

private:
	void process_pending_scene();

	std::optional<std::unique_ptr<IScene>> pending_new_scene_;
	std::unique_ptr<IScene> current_scene_;

	std::unique_ptr<sf::RenderWindow> window_;
};