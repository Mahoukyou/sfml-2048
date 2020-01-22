#include "game.h"

#include "mainscene.h"
#include "scenemanager.h"

Game::Game() :
	window_{ std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 800), "SFML works!") }
{
	window_->setFramerateLimit(60);
	
	SceneManager::instance().set_new_scene(std::make_unique<MainScene>());
}

void Game::main_loop()
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