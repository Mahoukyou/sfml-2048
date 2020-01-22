#include "spritebutton.h"

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include <cassert>

SpriteButton::SpriteButton(const sf::Vector2u size) :
	size_{ size }
{
	
}

bool SpriteButton::process_event(const sf::Event& event)
{
	if (state_ == e_state::disabled)
	{
		return false;
	}

	// todo MOUSE POS RELATIVE TO WINDOW
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:		
		if (state_ == e_state::hovered && is_mouse_in_area(0,0))
		{
			state_ = e_state::pressed;
			// todo, on pressed?
		}

		return true;

	case sf::Event::MouseButtonReleased:
		if (state_ == e_state::pressed && is_mouse_in_area(0,0))
		{
			state_ = e_state::normal;
			if (on_clicked)
			{
				on_clicked();
			}
		}
		
		return true;

	case sf::Event::MouseMoved:
	{
		const auto mouse_in_area = is_mouse_in_area(0,0);
		if (mouse_in_area && state_ == e_state::normal)
		{
			state_ = e_state::hovered;
			if (on_hovered)
			{
				on_hovered();
			}
		}
		else if (!mouse_in_area && state_ == e_state::hovered)
		{
			state_ = e_state::normal;
			if (on_unhovered)
			{
				on_unhovered();
			}
		}

		return true;
	}
		
	default:
		return false;
	}
}

void SpriteButton::set_texture(const sf::Texture& texture, e_state state)
{
	assert(static_cast<unsigned>(state) < static_cast<unsigned>(e_state::max) && "Invalid button state");

	auto sprite = std::make_unique<sf::Sprite>();
	sprite->setTexture(texture);

	const auto texture_size = sprite->getTexture()->getSize();
	sprite->setScale(static_cast<float>(size_.x) / texture_size.x, static_cast<float>(size_.y) / texture_size.y);

	sprites_[static_cast<int>(state)] = std::move(sprite);
}

void SpriteButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	const auto sprite = get_sprite();
	if (sprite)
	{
		states.transform *= getTransform();
		target.draw(*sprite, states);
	}
}

bool SpriteButton::is_mouse_in_area(const int mouse_x, const int mouse_y) const noexcept
{
	sf::FloatRect rect{
		getPosition().x,
		getPosition().y,
		static_cast<float>(size_.x),
		static_cast<float>(size_.y) };

	rect = getTransform().transformRect(rect);
	return rect.contains({static_cast<float>(mouse_x), static_cast<float>(mouse_y)});
}

sf::Sprite* SpriteButton::get_sprite() const noexcept
{
	const auto sprite = get_sprite(state_);

	return sprite ? sprite : get_sprite(e_state::normal);
}

sf::Sprite* SpriteButton::get_sprite(const e_state state) const noexcept
{
	assert(static_cast<unsigned>(state) < static_cast<unsigned>(e_state::max) && "Invalid button state");

	return sprites_[static_cast<unsigned>(state)].get();
}
