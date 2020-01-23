#include "board.h"

#include "resourcemanager.h"
#include "SFML/Graphics.hpp"
#include <stdexcept>
#include <random>
#include <cassert>


// TODO, make the size a single value, since it's always gonna be x by x
Board::Board(const unsigned size, const sf::Vector2f& render_size, const unsigned max_value) :
	size_{ size },
	max_value_{ max_value }
{
	if (size < 2)
	{
		// todo, custom exception (passing the invalid size)
		throw std::invalid_argument{ "Board size is not valid" };
	}

	board_.resize(size * size);

	init_background_tiles();
	set_render_size(render_size);
}

unsigned Board::size() const noexcept
{
	return size_;
}

unsigned Board::max_value() const noexcept
{
	return max_value_;
}

unsigned Board::score() const noexcept
{
	return score_;
}

void Board::set_render_size(const sf::Vector2f& render_size)
{
	background_.setSize(render_size);

	tile_size_ = get_tile_size(render_size);
	for (unsigned x = 0; x < size(); ++x)
	{
		for (unsigned y = 0; y < size(); ++y)
		{
			const auto index = xy_to_index(x, y);

			background_tiles_[index].setSize(tile_size_);
			background_tiles_[index].setPosition(get_tile_position(x, y));
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

	board_[random_empty_tile] = new_tile(index_to_xy(random_empty_tile), new_tile_value);

	return true;
}

std::vector<size_t> Board::get_empty_tiles()
{
	// We could reserve space in the vector,
	// but <20 size_t's and the board is usually gonna be halfway full so gonna levve it as it is
	std::vector<size_t> empty_tiles;
	for (size_t i = 0; i < board_.size(); ++i)
	{
		if (board_[i].value == 0)
		{
			empty_tiles.push_back(i);
		}
	}

	return empty_tiles;
}

bool Board::move(e_direction direction)
{
	const auto direction_vector{ get_direction_vector(direction) };
	
	const auto merge_result = merge_tiles(direction_vector);
	const auto move_result = move_tiles(direction_vector);

	return merge_result || move_result;
}

bool Board::any_moves_available() const
{
	return has_empty_tile() || any_merge_available();
}

bool Board::contains_value(const unsigned value) const
{
	const auto result = std::find_if(board_.begin(), board_.end(), [value](const Tile& tile) { return tile.value == value; });
	return result != board_.end();
}

void Board::clear()
{
	for (auto& tile : board_)
	{
		tile = {};
	}
	score_ = 0;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(background_, states);

	for (const auto& background_tile : background_tiles_)
	{
		target.draw(background_tile, states);
	}
	
	for(const auto& tile : board_)
	{
		target.draw(tile.sprite, states);
	}
}

Tile& Board::tile(const size_t x, const size_t y) noexcept
{
	assert(x < size() && y < size() && "Invalid x or y position");
	return board_[xy_to_index(x, y)];
}

const Tile& Board::tile(const size_t x, const size_t y) const noexcept
{
	assert(x < size() && y < size() && "Invalid x or y position");
	return board_[xy_to_index(x, y)];
}

Tile& Board::tile(const size_t index) noexcept
{
	assert(index < board_.size() && "Invalid index");
	return board_[index];
}

const Tile& Board::tile(const size_t index) const noexcept
{
	assert(index < board_.size() && "Invalid index");
	return board_[index];
}

void Board::init_background_tiles()
{
	background_.setFillColor(sf::Color{ 189, 174, 136 });

	sf::RectangleShape tile;
	tile.setFillColor(sf::Color{ 232, 212, 160 });
	background_tiles_.resize(size() * size(), tile);
}

sf::Vector2f Board::get_tile_size(const sf::Vector2f& render_size) const noexcept
{
	const sf::Vector2f total_padding{ size() * tile_padding_, size() * tile_padding_ };
	const auto useful_area = render_size - total_padding;

	return { useful_area.x / size(), useful_area.y / size() };
}

sf::Vector2f Board::get_tile_position(const unsigned x, const unsigned y) const noexcept
{
	return {
		x * tile_size_.x + x * tile_padding_ + tile_padding_ / 2.0f,
		y * tile_size_.y + y * tile_padding_ + tile_padding_ / 2.0f };
}

size_t Board::xy_to_index(const size_t x, const size_t y) const noexcept
{
	return y * size() + x;
}

sf::Vector2i Board::index_to_xy(size_t index) const noexcept
{
	sf::Vector2i result;
	result.x = static_cast<int>(index % size());
	result.y = static_cast<int>(index / size());

	return result;
}

sf::Vector2i Board::get_direction_vector(const e_direction direction) const
{
	// We could also use non static constexpr std::array and static cast direction to int
	// for better performance, but it's more error prone (i.e. when we change order in the enum)
	// either static map or make it a member variable {OR SWITCH IT UP}
	static const std::map<e_direction, sf::Vector2i> direction_vectors
	{
			{e_direction::NORTH, {0, 1}},
			{e_direction::SOUTH, {0, -1}},
			{e_direction::WEST, {1, 0}},
			{e_direction::EAST, {-1, 0}}
	};

	return direction_vectors.find(direction)->second;
}

std::optional<sf::Vector2i> Board::get_next_position(sf::Vector2i position, const sf::Vector2i direction_vector) const
{
	position += direction_vector;
	if (position.x < 0 || position.y < 0 ||
		position.x >= static_cast<int>(size()) || position.y >= static_cast<int>(size()))
	{
		return std::nullopt;
	}

	return position;
}

std::optional<sf::Vector2i> Board::get_next_non_empty_position(const sf::Vector2i position, const sf::Vector2i direction_vector) const
{
	const auto is_empty = [&](const sf::Vector2i& pos)
	{
		return tile(pos.x, pos.y).value == 0;
	};

	auto next_not_empty_position = get_next_position(position, direction_vector);
	while (next_not_empty_position &&
		is_empty(next_not_empty_position.value()))
	{
		next_not_empty_position = get_next_position(next_not_empty_position.value(), direction_vector);
	}

	if (!next_not_empty_position || is_empty(next_not_empty_position.value()))
	{
		return std::nullopt;
	}

	return next_not_empty_position;
}

std::pair<std::vector<size_t>, std::vector<size_t>> Board::get_sequence_vectors(const sf::Vector2i direction_vector) const
{
	std::vector<size_t> x_vec, y_vec;
	x_vec.reserve(size());
	y_vec.reserve(size());
	for (size_t i = 0; i < size(); ++i)
	{
		x_vec.push_back(i);
		y_vec.push_back(i);
	}

	if (direction_vector.x == -1)
	{
		std::reverse(x_vec.begin(), x_vec.end());
	}
	if (direction_vector.y == -1)
	{
		std::reverse(y_vec.begin(), y_vec.end());
	}

	return std::make_pair(std::move(x_vec), std::move(y_vec));
}

bool Board::has_empty_tile() const
{
	const auto result = std::find_if(board_.begin(), board_.end(), [](const Tile& tile) { return tile.value == 0; });
	return result != board_.end();
}

bool Board::any_merge_available() const
{
	std::vector<sf::Vector2i> directions_vector;
	directions_vector.reserve(4);
	directions_vector.push_back(get_direction_vector(e_direction::NORTH));
	directions_vector.push_back(get_direction_vector(e_direction::SOUTH));
	directions_vector.push_back(get_direction_vector(e_direction::WEST));
	directions_vector.push_back(get_direction_vector(e_direction::EAST));
	
	for (size_t x = 0; x < size(); ++x)
	{
		for (size_t y = 0; y < size(); ++y)
		{
			if (tile(x, y).value == 0)
			{
				continue;
			}

			for (const auto direction : directions_vector)
			{
				const auto next_position = get_next_position({ static_cast<int>(x), static_cast<int>(y) }, direction);
				if (!next_position)
				{
					continue;;
				}

				const auto [next_x, next_y] = next_position.value();
				if (tile(x,y).value == tile(next_x, next_y).value)
				{
					return true;
				}
			}
		}
	}

	return false;
}

Tile Board::new_tile(const sf::Vector2i position, const unsigned value) const
{
	Tile tile{};
	if (value != 0)
	{
		const auto texture = ResourceManager::instance().get_texture(std::to_string(value));
		if (!texture)
		{
			throw std::runtime_error{ "Texture not loaded" };
		}
		
		tile.sprite.setTexture(*texture);
		tile.sprite.setPosition(get_tile_position(position.x, position.y));
		const auto size = tile.sprite.getTexture()->getSize();
		tile.sprite.setScale(tile_size_.x / size.x, tile_size_.y / size.y);
	}
	tile.value = value;

	return tile;
}

bool Board::merge_tiles(const sf::Vector2i direction_vector)
{
	bool any_merged{ false };

	const auto [x_vector, y_vector] = get_sequence_vectors(direction_vector);
	for (const auto x : x_vector)
	{
		for (const auto y : y_vector)
		{
			if (const auto tile_value = tile(x, y).value;
				tile_value == 0 || tile_value == max_value())
			{
				continue;
			}

			auto next_not_empty_position = get_next_non_empty_position({ static_cast<int>(x), static_cast<int>(y) }, direction_vector);
			if (!next_not_empty_position)
			{
				continue;
			}

			const auto [next_x, next_y] = next_not_empty_position.value();
			if (tile(x, y).value == tile(next_x, next_y).value)
			{
				tile(x, y) = new_tile({static_cast<int>(x), static_cast<int>(y)}, tile(x,y).value * 2);
				tile(next_x, next_y) = {};

				score_ += tile(x, y).value;
				any_merged = true;
			}
		}
	}

	return any_merged;
}

bool Board::move_tiles(const sf::Vector2i direction_vector)
{
	bool any_moved{ false };

	const auto [x_vector, y_vector] = get_sequence_vectors(direction_vector);
	for (const auto x : x_vector)
	{
		for (const auto y : y_vector)
		{
			if (tile(x, y).value != 0)
			{
				continue;
			}

			const auto next_non_empty_position{ get_next_non_empty_position({static_cast<int>(x), static_cast<int>(y)}, direction_vector) };
			if (!next_non_empty_position)
			{
				continue;
			}

			const auto [new_x, new_y] = next_non_empty_position.value();
			tile(x, y) = new_tile({static_cast<int>(x), static_cast<int>(y)}, tile(new_x, new_y).value);
			tile(new_x, new_y) = {};
			
			any_moved = true;
		}
	}

	return any_moved;
}
