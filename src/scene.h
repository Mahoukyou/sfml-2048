#pragma once

namespace sf
{
	class Event;
	class RenderWindow;
}

class IScene
{
public:
	IScene() = default;
	virtual ~IScene() = default;

	IScene(const IScene&) = delete;
	IScene(IScene&&) = delete;
	
	IScene& operator=(const IScene&) = delete;
	IScene& operator=(IScene&&) = delete;

	// TODO, should I use boolean to indicate if event was processed?
	virtual void process_event(const sf::Event& event) = 0;
	virtual void update(float delta_time) = 0;
	virtual void render(sf::RenderWindow& target) = 0;
};