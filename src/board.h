#pragma once

#include "scene.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Drawable.hpp"

#include <optional>


// tmp
#include "SFML/Graphics.hpp"



class Board : public sf::Drawable, public sf::Transformable
{
public:
	enum class e_direction
	{
		NORTH,
		SOUTH,
		WEST,
		EAST
	};

	enum class e_axis
	{
		vertical,
		horizontal
	};
	
	Board(const sf::Vector2u& size, const sf::Vector2f& render_size);
	~Board() = default;

	[[nodiscard]] const sf::Vector2u& size() const noexcept;

	void set_render_size(const sf::Vector2f& render_size);

	bool spawn_new_tile();
	[[nodiscard]] std::vector<size_t> get_empty_tiles();

	bool move(e_direction direction);
	
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void init_background_tiles();

	[[nodiscard]] sf::Vector2f get_tile_size(const sf::Vector2f& render_size) const noexcept;
	[[nodiscard]] sf::Vector2f get_tile_position(unsigned x, unsigned y) const noexcept;
	[[nodiscard]] size_t xy_to_index(size_t x, size_t y) const noexcept;

	[[nodiscard]] sf::Vector2i get_direction_vector(e_direction direction) const;
	[[nodiscard]] std::optional<sf::Vector2i> get_next_position(sf::Vector2i position, sf::Vector2i direction_vector) const;
	[[nodiscard]] std::optional<sf::Vector2i> get_next_non_empty_position(sf::Vector2i position, sf::Vector2i direction_vector) const;
	[[nodiscard]] std::pair<std::vector<size_t>, std::vector<size_t>> get_sequence_vectors(sf::Vector2i direction_vector) const;
	
	bool merge_tiles(sf::Vector2i direction_vector);
	bool move_tiles(sf::Vector2i direction_vector);
private:
	sf::Vector2u size_;
	std::vector<unsigned> board_;

	// TODO
	float tile_padding_{ 10 };
	sf::Vector2f tile_size_{};
	sf::RectangleShape background_;
	std::vector<sf::RectangleShape> empty_tiles_;
};
