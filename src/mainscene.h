#pragma once

#include "scene.h"
#include "board.h"

class MainScene : public IScene
{
public:
	MainScene();
	~MainScene() override = default;

	void process_event(const sf::Event& event) override;
	void update(float delta_time) override;
	void render(sf::RenderWindow& target) override;
	
private:
	bool finish_on_max_value_{ true };
	const unsigned max_value_{ };
	Board board_;

	std::optional<Board::e_direction> pending_move{ std::nullopt };
};
