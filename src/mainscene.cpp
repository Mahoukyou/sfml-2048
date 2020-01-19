#include "mainscene.h"

#include "resourcemanager.h"

#include "SFML/Graphics.hpp"

MainScene::MainScene() :
	board_{ {15, 15 }, {600, 600} }
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
	//
	//board_.spawn_new_tile();
	//board_.spawn_new_tile();
	//
	//board_.spawn_new_tile();
	//board_.spawn_new_tile();
	//
	//board_.spawn_new_tile();
	//board_.spawn_new_tile();
	//
	//board_.spawn_new_tile();
	//board_.spawn_new_tile();
}

void MainScene::process_event(const sf::Event& event)
{
	// todo ,move it into pudate, just handle the input here

	bool moved = false;
	if(event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::W:
			moved = board_.move(Board::e_direction::NORTH);
			break;
		case sf::Keyboard::S:
			moved = board_.move(Board::e_direction::SOUTH);
			break;
		case sf::Keyboard::A:
			moved = board_.move(Board::e_direction::WEST);
			break;
		case sf::Keyboard::D:
			moved = board_.move(Board::e_direction::EAST);
			break;
			
		default:;
		}
	}

	if(moved)
	{
		board_.spawn_new_tile();
	}
}

void MainScene::update(const float delta_time)
{

}

void MainScene::render(sf::RenderWindow& target)
{
	target.clear();
	target.draw(board_);
	target.display();
}
