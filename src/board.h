#pragma once

#include "scene.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Drawable.hpp"


// tmp
#include "SFML/Graphics.hpp"



class Board : public sf::Drawable
{
public:
	Board(const sf::Vector2u& size);
	~Board() = default;

	[[nodiscard]] const sf::Vector2u& size() const noexcept;
	
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setup_background_tiles();
	// todo, public (pass render size of the board)
	void set_render_size();
	[[nodiscard]] sf::Vector2f get_tile_size() const noexcept;

	size_t xy_to_index(unsigned x, unsigned y) const noexcept;
	
private:
	sf::Vector2u size_;

	// TODO
	float tile_padding_{ 10 };
	sf::RectangleShape background_;
	std::vector<sf::RectangleShape> empty_tiles_;

	// todo, use window size?
	sf::Vector2f render_size_{ 600, 600 };
};
