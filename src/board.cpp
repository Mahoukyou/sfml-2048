#include "board.h"

#include "SFML/Graphics.hpp"
#include <stdexcept>

Board::Board(const sf::Vector2u& size) :
	size_{ size }
{
	if(size.x < 2 || size.y < 2)
	{
		// todo, custom exception (passing the invalid size)
		throw std::invalid_argument{ "Board size is not valid" };
	}

	setup_background_tiles();
}

const sf::Vector2u& Board::size() const noexcept
{
	return size_;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background_);

	for(const auto& tile : empty_tiles_)
	{
		target.draw(tile);
	}
}

void Board::setup_background_tiles()
{
	background_.setFillColor(sf::Color{ 189, 174, 136 });

	sf::RectangleShape tile;
	tile.setFillColor(sf::Color{ 232, 212, 160 });
	empty_tiles_.resize(size().x * size().y, tile);

	set_render_size(); // todo, do (recalculate/resize background) instead of set render size
}

void Board::set_render_size()
{
	//render_size_ = ... todo, for now its static

	background_.setSize(render_size_);
	
	const auto tile_size = get_tile_size();
	for (unsigned x = 0; x < size().x; ++x)
	{
		for (unsigned y = 0; y < size().y; ++y)
		{
			const auto index = xy_to_index(x, y);

			empty_tiles_[index].setSize(tile_size);
			empty_tiles_[index].setPosition(
				x * tile_size.x + x * tile_padding_ + tile_padding_ / 2.0f,
				y * tile_size.y + y * tile_padding_ + tile_padding_ / 2.0f);
		}
	}
}

sf::Vector2f Board::get_tile_size() const noexcept
{
	
	const sf::Vector2f total_padding = {size().x * tile_padding_, size().y * tile_padding_};
	const auto useful_area = render_size_ - total_padding;

	return { useful_area.x / size().x, useful_area.y / size().y };
}

size_t Board::xy_to_index(const unsigned x, const unsigned y) const noexcept
{
	return y * size().x + x;
}
