#include "board.h"

#include "resourcemanager.h"
#include "SFML/Graphics.hpp"
#include <stdexcept>
#include <random>
#include <optional>


// TODO, make the size a single value, since it's always gonna be x by x
Board::Board(const sf::Vector2u& size, const sf::Vector2f& render_size) :
	size_{ size }
{
	if (size.x < 2 || size.y < 2)
	{
		// todo, custom exception (passing the invalid size)
		throw std::invalid_argument{ "Board size is not valid" };
	}

	board_.resize(size.x * size.y, 0);

	init_background_tiles();
	set_render_size(render_size);
}

const sf::Vector2u& Board::size() const noexcept
{
	return size_;
}

void Board::set_render_size(const sf::Vector2f& render_size)
{
	background_.setSize(render_size);

	tile_size_ = get_tile_size(render_size);
	for (unsigned x = 0; x < size().x; ++x)
	{
		for (unsigned y = 0; y < size().y; ++y)
		{
			const auto index = xy_to_index(x, y);

			empty_tiles_[index].setSize(tile_size_);
			empty_tiles_[index].setPosition(get_tile_position(x, y));
		}
	}
}

bool Board::spawn_new_tile()
{
	// todo, rename the empty tiles here or the background oens (since it is confusing)
	std::vector<size_t> empty_tiles{ get_empty_tiles() };

	if (empty_tiles.empty())
	{
		return false;
	}

	std::random_device rd{};
	std::mt19937 gen{ rd() };

	const std::uniform_int_distribution<unsigned> tile_position_dist{ 0, static_cast<unsigned>(empty_tiles.size() - 1) };
	const auto random_empty_tile = empty_tiles[tile_position_dist(gen)];

	const std::uniform_int_distribution<> tile_value_dist{ 1, 2 };
	const unsigned new_tile_value = tile_value_dist(gen) * 2;

	// todo, spawn tile
	board_[random_empty_tile] = new_tile_value;

	return true;
}

std::vector<size_t> Board::get_empty_tiles()
{
	// We could reserve space in the vector,
	// but <20 size_t's and the board is usually gonna be halfway full so gonna levve it as it is
	std::vector<size_t> empty_tiles;
	for (size_t i = 0; i < board_.size(); ++i)
	{
		if (board_[i] == 0)
		{
			empty_tiles.push_back(i);
		}
	}

	return empty_tiles;
}

bool Board::move(e_direction direction)
{
	const auto merge_result = merge_tiles(direction);
	const auto move_result = move_tiles(direction);

	return merge_result || move_result;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(background_, states);

	for (const auto& tile : empty_tiles_)
	{
		target.draw(tile, states);
	}



	// todo, test
	sf::Sprite tile_sprite{};
	for (unsigned x = 0; x < size().x; ++x)
	{
		for (unsigned y = 0; y < size().y; ++y)
		{
			const unsigned tile = board_[xy_to_index(x, y)];
			if (tile != 0)
			{
				tile_sprite.setTexture(*ResourceManager::instance().get_texture(std::to_string(tile)));
				tile_sprite.setPosition(get_tile_position(x, y));
				const auto size = tile_sprite.getTexture()->getSize();
				tile_sprite.setScale(tile_size_.x / size.x, tile_size_.y / size.y);
				target.draw(tile_sprite);
			}
		}
	}
}

void Board::init_background_tiles()
{
	background_.setFillColor(sf::Color{ 189, 174, 136 });

	sf::RectangleShape tile;
	tile.setFillColor(sf::Color{ 232, 212, 160 });
	empty_tiles_.resize(size().x * size().y, tile);
}

sf::Vector2f Board::get_tile_size(const sf::Vector2f& render_size) const noexcept
{
	const sf::Vector2f total_padding{ size().x * tile_padding_, size().y * tile_padding_ };
	const auto useful_area = render_size - total_padding;

	return { useful_area.x / size().x, useful_area.y / size().y };
}

sf::Vector2f Board::get_tile_position(const unsigned x, const unsigned y) const noexcept
{
	return {
		x * tile_size_.x + x * tile_padding_ + tile_padding_ / 2.0f,
		y * tile_size_.y + y * tile_padding_ + tile_padding_ / 2.0f };
}

size_t Board::xy_to_index(const unsigned x, const unsigned y) const noexcept
{
	return y * size().x + x;
}

std::pair<std::vector<size_t>, std::vector<size_t>> Board::get_sequence_vectors(const e_direction direction)
{
	std::vector<size_t> x_vec, y_vec;
	x_vec.reserve(size().x);
	y_vec.reserve(size().y);
	for(size_t x = 0; x < size().x; ++x)
	{
		x_vec.push_back(x);
	}

	for (size_t y = 0; y < size().y; ++y)
	{
		y_vec.push_back(y);
	}

	// todo, move to difeerent function (since we are using the same vectors in getnexttpos)
	const std::map<e_direction, sf::Vector2<int>> direction_vectors
	{
			{e_direction::NORTH, {0, 1}},
			{e_direction::SOUTH, {0, -1}},
			{e_direction::WEST, {1, 0}},
			{e_direction::EAST, {-1, 0}}
	};
	const auto direction_vector = direction_vectors.find(direction)->second;
	if(direction_vector.x == -1)
	{
		std::reverse(x_vec.begin(), x_vec.end());
	}

	if (direction_vector.y == -1)
	{
		std::reverse(y_vec.begin(), y_vec.end());
	}

	return std::make_pair(std::move(x_vec), std::move(y_vec));
}

bool Board::merge_tiles(const e_direction direction)
{
	bool any_merged{ false };


	// TODO, FIX TYPES, SINCE TYPE MISMATCH IS ALL OVER THE PLACE
	const auto get_next_position = [&](int x, int y) -> std::optional<std::pair<size_t, size_t>>
	{
		// TODO, MMAKE A SINGLE GET DIRECTION VECTOR FUNC
		const std::map<e_direction, sf::Vector2<int>> direction_vectors
		{
			{e_direction::NORTH, {0, 1}},
			{e_direction::SOUTH, {0, -1}},
			{e_direction::WEST, {1, 0}},
			{e_direction::EAST, {-1, 0}}
		};
		const auto direction_vector = direction_vectors.find(direction)->second;

		x += direction_vector.x;
		y += direction_vector.y;

		if (x < 0 || y < 0 || x >= size().x || y >= size().y)
		{
			return std::nullopt;
		}

		return std::make_pair(x, y);

	};

	const auto is_empty = [&](std::pair<size_t, size_t> t)
	{
		const auto [x, y] = t;
		return board_[xy_to_index(x, y)] == 0;
	};
	
	const auto [x_vector, y_vector] = get_sequence_vectors(direction);
	for (const auto x : x_vector)
	{
		for (const auto y : y_vector)
		{
			// todo, cehck for max value!
			if (board_[xy_to_index(x, y)] == 0)
			{
				continue;
			}
			
			auto next_not_empty_position = get_next_position(x, y);
			while (next_not_empty_position &&
				is_empty(next_not_empty_position.value()))
			{
				const auto [next_x, next_y] = next_not_empty_position.value();
				next_not_empty_position = get_next_position(next_x, next_y);
			} 

			if (!next_not_empty_position || is_empty(next_not_empty_position.value()))
			{
				continue;
			}

			const auto [next_x, next_y] = next_not_empty_position.value();
			if (board_[xy_to_index(x, y)] == board_[xy_to_index(next_x, next_y)])
			{
				board_[xy_to_index(x, y)] *= 2;
				board_[xy_to_index(next_x, next_y)] = 0;

				any_merged = true;
			}
		}
	}
	
	return any_merged;
}

bool Board::move_tiles(e_direction direction)
{
	bool any_moved{ false };
	
	switch (direction)
	{
	case e_direction::NORTH:
		for (unsigned x = 0; x < size().x; ++x)
		{
			for (unsigned y_begin = 0; y_begin < size().y; ++y_begin)
			{
				if (board_[xy_to_index(x, y_begin)] != 0)
				{
					continue;
				}

				bool any_local_moved{ false };
				for (unsigned y = y_begin + 1; y < size().y; ++y)
				{
					if (board_[xy_to_index(x, y)] != 0)
					{
						std::swap(board_[xy_to_index(x, y_begin)], board_[xy_to_index(x, y)]);
						any_moved = true;
						any_local_moved = true;
						break;
					}
				}

				if (!any_local_moved)
				{
					break;
				}
			}
		}
		break;

	case e_direction::SOUTH:
		for (unsigned x = 0; x < size().x; ++x)
		{
			for (int y_begin = size().y - 1; y_begin >= 0; --y_begin)
			{
				if (board_[xy_to_index(x, y_begin)] != 0)
				{
					continue;
				}

				bool any_local_moved{ false };
				for (int y = y_begin - 1; y >= 0; --y)
				{
					if (board_[xy_to_index(x, y)] != 0)
					{
						std::swap(board_[xy_to_index(x, y_begin)], board_[xy_to_index(x, y)]);
						any_moved = true;
						any_local_moved = true;
						break;
					}
				}

				if (!any_local_moved)
				{
					break;
				}
			}
		}
		break;

	case e_direction::WEST:
		for (unsigned y = 0; y < size().y; ++y)
		{
			for (unsigned x_begin = 0; x_begin < size().x; ++x_begin)
			{
				if (board_[xy_to_index(x_begin, y)] != 0)
				{
					continue;
				}

				bool any_local_moved{ false };
				for (unsigned x = x_begin + 1; x < size().x; ++x)
				{
					if (board_[xy_to_index(x, y)] != 0)
					{
						std::swap(board_[xy_to_index(x_begin, y)], board_[xy_to_index(x, y)]);
						any_moved = true;
						any_local_moved = true;
						break;
					}
				}

				if (!any_local_moved)
				{
					break;
				}
			}
		}
		break;
		
	case e_direction::EAST:
		for (unsigned y = 0; y < size().y; ++y)
		{
			for (int x_begin = size().x - 1; x_begin >= 0; --x_begin)
			{
				if (board_[xy_to_index(x_begin, y)] != 0)
				{
					continue;
				}

				bool any_local_moved{ false };
				for (int x = x_begin - 1; x >= 0; --x)
				{
					if (board_[xy_to_index(x, y)] != 0)
					{
						std::swap(board_[xy_to_index(x_begin, y)], board_[xy_to_index(x, y)]);
						any_moved = true;
						any_local_moved = true;
						break;
					}
				}

				if (!any_local_moved)
				{
					break;
				}
			}
		}
		break;

	default:; // todo
	}
	

	return any_moved;
}
