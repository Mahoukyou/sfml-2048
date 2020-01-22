#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"

#include <array>
#include <functional>
#include <memory>

namespace sf
{
	class Event;
	class Sprite;
}

class SpriteButton : public sf::Drawable, public sf::Transformable
{
public:
	enum class e_state
	{
		normal,
		hovered,
		pressed,
		disabled,

		max
	};
	
	SpriteButton(sf::Vector2u size);
	virtual ~SpriteButton() = default;

	SpriteButton(const SpriteButton&) = delete; // todo, custom copy
	SpriteButton(SpriteButton&&) = default;
	
	SpriteButton& operator=(const SpriteButton&) = delete; // todo, custom copy
	SpriteButton& operator=(SpriteButton&&) = default;

	virtual bool process_event(const sf::Event& event);

	void set_texture(const sf::Texture& texture, e_state state);

	std::function<void()> on_clicked;
	std::function<void()> on_hovered;
	std::function<void()> on_unhovered;
	
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	[[nodiscard]] bool is_mouse_in_area(int mouse_x, int mouse_y) const noexcept;
	sf::Sprite* get_sprite() const noexcept;
	sf::Sprite* get_sprite(e_state state) const noexcept;
	
	e_state state_{ e_state::normal };
	sf::Vector2u size_; // TODO Should I use this size or state specific size (from received sf::sprite)?

	std::array<std::unique_ptr<sf::Sprite>, static_cast<unsigned>(e_state::max)> sprites_;
};