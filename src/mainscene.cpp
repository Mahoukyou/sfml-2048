#include "mainscene.h"

#include <SFML/Graphics.hpp>

MainScene::MainScene() :
	board_{ {4, 4 } }
{
	
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
