#pragma once

#include <SFML/Graphics.hpp>

class Cursor
{
public:
	Cursor();

	void RenderTo(sf::RenderTexture* renderTarget);
	void SetPosition(sf::Vector2f position);

private:
	sf::RectangleShape m_visual;
};