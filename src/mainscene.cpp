#include "mainscene.h"

#include "resourcemanager.h"

#include "SFML/Graphics.hpp"

MainScene::MainScene() :
	board_{ {4, 4 }, {600, 600} }
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
