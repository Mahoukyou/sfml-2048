#include "mainscene.h"

#include "resourcemanager.h"

#include "SFML/Graphics.hpp"
#include <iostream>

MainScene::MainScene() :
	max_value_{2048},
	board_{ 16, {600, 600}, max_value_ }
{
	// preload the textures atm so we won't have to deal with filenames in board
	for(int i = 2; i <= 8192; i*=2)
	{
		const std::string file_name = std::to_string(i);
		// todo, local file path
		const std::string file = R"(K:\Development\SFML-2048\res\)" + file_name + ".png";
		ResourceManager::instance().load_texture(file, file_name);
	}
	
	board_.spawn_new_tile();
	board_.spawn_new_tile();
}

void MainScene::process_event(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::W:
			pending_move = Board::e_direction::NORTH;
			break;
		case sf::Keyboard::S:
			pending_move = Board::e_direction::SOUTH;
			break;
		case sf::Keyboard::A:
			pending_move = Board::e_direction::WEST;
			break;
		case sf::Keyboard::D:
			pending_move = Board::e_direction::EAST;
			break;
			
		default:;
		}
	}
}

void MainScene::update(const float /*delta_time*/)
{
	if (pending_move)
	{
		const auto moved = board_.move(pending_move.value());
		if (moved)
		{
			board_.spawn_new_tile();
		}

		if (finish_on_max_value_ && board_.contains_value(max_value_))
		{
			std::cout << "WON";
		}
		else if (!board_.any_moves_available())
		{
			std::cout << "END GMAE";
		}

		pending_move = std::nullopt;
	}
}

void MainScene::render(sf::RenderWindow& target)
{
	target.clear();
	target.draw(board_);
	target.display();
}
