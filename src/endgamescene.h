#pragma once

#include "scene.h"

class EndGameScene : public IScene
{
public:
	enum class e_end_game_state
	{
		WON,
		LOST
	};
	
	EndGameScene(e_end_game_state state, unsigned score);
	~EndGameScene() override = default;

	void process_event(const sf::Event& event) override;
	void update(float delta_time) override;
	void render(sf::RenderWindow& target) override;
	
private:
	e_end_game_state end_game_state_;
	const unsigned score_;
};
