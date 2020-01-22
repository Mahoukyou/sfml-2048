#include "game.h"

#include "mainscene.h"
#include "scenemanager.h"

#include <cassert>
#include <atomic>

Game::Game() :
	window_{ std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 800), "SFML works!") }
{
	window_->setFramerateLimit(60);
	
	SceneManager::instance().set_new_scene(std::make_unique<MainScene>());
}

Game& Game::instance()
{
	static Game game{};
	return game;
}

const sf::RenderWindow& Game::window() const noexcept
{
	assert(window_ && "Window should be valid");
	return *window_;
}

void Game::main_loop()
{
	// I am lazy (and bad at design :D) and making the Game a singleton, so we can get access to Window and other stuff we might store in here
	// Making sure main_loop is called only once
	// std::call_once won't work here, since we are not returning from the main_loop until end.
	
	static std::atomic_bool called{ false };
	if (!called.exchange(true))
	{
		internal_main_loop();
	}
	else
	{
		throw std::runtime_error{ "main_loop called more than once" };	
	}
}

void Game::internal_main_loop()
{
	sf::Clock delta_clock;

	auto& scene_manager = SceneManager::instance();
	while (window_->isOpen())
	{
		const auto delta_time = delta_clock.restart();
		scene_manager.process_pending_scene();

		sf::Event event{};
		while (window_->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window_->close();
			}

			if (scene_manager.current_scene())
			{
				scene_manager.current_scene()->process_event(event);
			}
		}

		if (scene_manager.current_scene())
		{
			scene_manager.current_scene()->update(delta_time.asSeconds());
			scene_manager.current_scene()->render(*window_);
		}
	}
}