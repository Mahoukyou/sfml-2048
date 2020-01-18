#pragma once

#include <memory>
#include <map>
#include <string>

#include "SFML/Graphics/Texture.hpp"

class ResourceManager final
{
public:
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) noexcept = delete;

	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) noexcept = delete;

	static ResourceManager& instance();
	
	const sf::Texture* load_texture(const std::string& file, const std::string& id);
	const sf::Texture* get_texture(const std::string& id);

private:
	ResourceManager() = default;

	// for now persistent unique ptr(don't even need a ptr though) is enough (small enough game so the resources can be allocated throughout)
	// todo maybe later do some selfpurge once the resource is no longer used
	std::map<std::string, std::unique_ptr<sf::Texture>> textures_;	
};
