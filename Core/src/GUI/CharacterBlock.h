#pragma once

#include "Character.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

class CharacterBlock
{
public:
	void Start(uint32_t maxWidth);
	void ChangeWidth(uint32_t maxWidth);

	std::string GetText() const;
	void SetText(const std::string& text);

	void SetCharacter(Character* character);

	void RenderTo(sf::RenderTexture* renderTarget);

	uint32_t GetHeight() const;

private:
	void Recalculate();

	uint32_t m_maxWidth = 100;
	uint32_t m_charMax = 100;
	uint32_t m_height = 1;

	std::string m_contents;

	Character* m_character = nullptr;
	sf::Text m_textVisual;
	sf::Text m_headerVisual;
	sf::RectangleShape m_colorBar;
	sf::RectangleShape m_headerBar;
	sf::RenderTexture m_renderTarget;
	sf::Sprite m_sprite;
};