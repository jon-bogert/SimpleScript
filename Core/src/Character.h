#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>

struct Character
{
	std::string name;
	std::string notes;
	sf::Color color = sf::Color::Red;
};