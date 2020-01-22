#include "mainscene.h"

#include "resourcemanager.h"
#include "scenemanager.h"
#include "endgamescene.h"

#include "SFML/Graphics.hpp"

#include <iostream>

MainScene::MainScene() :
	max_value_{ 2048 },
	board_{ 4, {600, 600}, max_value_ },
	sb{{100, 100}}
{
	// preload the textures atm so we won't have to deal with filenames in board
	for(int i = 2; i <= 8192; i*=2)
	{
		const std::string file_name = std::to_string(i);
		// todo, local file path
		const std::string file = R"(K:\Development\SFML-2048\res\)" + file_name + ".png";
		const auto result = ResourceManager::instance().load_texture(file, file_name);

		if(!result)
		{
			throw std::runtime_error{ "Texture couldn't be loaded" };
		}
	}
	
	board_.setPosition(0, 200);
	board_.spawn_new_tile();
	board_.spawn_new_tile();
	
	background_.setSize({ 600, 800 });
	background_.setFillColor({ 255, 248, 181 });

	init_score();

	sb.setPosition(50, 100);
	sb.set_texture(*ResourceManager::instance().get_texture("2"), SpriteButton::e_state::normal);
	sb.set_texture(*ResourceManager::instance().get_texture("4"), SpriteButton::e_state::hovered);
	sb.set_texture(*ResourceManager::instance().get_texture("8"), SpriteButton::e_state::pressed);


	sb.on_hovered = []() {std::cout << "HOVERED\n"; };
	sb.on_unhovered = []() {std::cout << "unhovered\n"; };
	sb.on_clicked = []() {std::cout << "clicked\n"; };
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

	sb.process_event(event);
}

void MainScene::update(const float /*delta_time*/)
{
	if (pending_move)
	{
		const auto moved = board_.move(pending_move.value());
		if (moved)
		{
			board_.spawn_new_tile();
			update_score();
		}

		if (finish_on_max_value_ && board_.contains_value(max_value_))
		{
			SceneManager::instance().set_new_scene(
				std::make_unique<EndGameScene>(EndGameScene::e_end_game_state::WON, board_.score()));
		}
		else if (!board_.any_moves_available())
		{
			SceneManager::instance().set_new_scene(
				std::make_unique<EndGameScene>(EndGameScene::e_end_game_state::LOST, board_.score()));
		}

		pending_move = std::nullopt;
	}
}

void MainScene::render(sf::RenderWindow& target)
{
	target.clear();
	target.draw(background_);
	target.draw(score_);
	target.draw(sb);
	target.draw(board_);
	target.display();
}

void MainScene::init_score()
{
	const std::string font_file = R"(K:\Development\SFML-2048\res\Montserrat-Regular.ttf)";
	const auto font{ ResourceManager::instance().load_font(font_file, "montserrat") };
	if (!font)
	{
		throw std::runtime_error{ "Font couldn't be loaded" };;
	}

	score_.setFont(*font);
	score_.setPosition(0, 0);
	score_.setCharacterSize(100);
	score_.setFillColor(sf::Color::Black);

	update_score();
}

void MainScene::update_score()
{
	score_.setString("Score: " + std::to_string(board_.score()));
}
