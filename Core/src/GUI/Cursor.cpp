#include "Cursor.h"

Cursor::Cursor()
{
	m_visual.setFillColor(sf::Color::White);
	m_visual.setSize({ 2.f, 32.f });
}

void Cursor::RenderTo(sf::RenderTexture* renderTarget)
{
	renderTarget->draw(m_visual);
}

void Cursor::SetPosition(sf::Vector2f position)
{
	m_visual.setPosition(position);
}
