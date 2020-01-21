#include "endgamescene.h"

#include "SFML/Graphics/RenderWindow.hpp"

#include <iostream>

EndGameScene::EndGameScene(const e_end_game_state state, const unsigned score) :
	end_game_state_{ state },
	score_{ score }
{

	std::cout << "You have " << (state == e_end_game_state::WON ? "Won" : "Lost") << "!\n";
	std::cout << "Score: " << score;
}

void EndGameScene::process_event(const sf::Event& event)
{
	
}

void EndGameScene::update(const float /*delta_time*/)
{
	
}

void EndGameScene::render(sf::RenderWindow& target)
{
	target.clear();
	target.display();
}
