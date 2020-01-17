#include "game.h"

#include "mainscene.h"

Game::Game() :
	window_{ std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML works!") }
{
	window_->setFramerateLimit(60);
	
	set_new_scene(std::make_unique<MainScene>());
}

void Game::main_loop()
{
	sf::Clock delta_clock;
	while (window_->isOpen())
	{
		const auto delta_time = delta_clock.restart();
		process_pending_scene();

		sf::Event event{};
		while (window_->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window_->close();
			}

			if(current_scene())
			{
				current_scene_->process_event(event);
			}
		}

		if (current_scene())
		{
			current_scene_->update(delta_time.asSeconds());
			current_scene_->render(*window_);
		}
	}
}

void Game::set_new_scene(std::unique_ptr<IScene> new_scene)
{
	pending_new_scene_ = std::move(new_scene);
}

const IScene* Game::current_scene() const noexcept
{
	return current_scene_.get();
}

void Game::process_pending_scene()
{
	if (pending_new_scene_)
	{
		current_scene_ = std::move(pending_new_scene_.value());
		pending_new_scene_ = std::nullopt;
	}
}
