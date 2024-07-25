#pragma once

#include "Character.h"
#include "RenderObject.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

class CharacterBlock : public RenderObject
{
public:
	void Start(uint32_t maxWidth);
	void ChangeWidth(uint32_t maxWidth) override;

	std::string GetText() const;
	void SetText(const std::string& text);
	void AppendText(const std::string& text);

	void SetCharacter(const std::string& name);
	std::string GetCharacter() const;

	void RenderTo(sf::RenderTexture* renderTarget) override;

	void SetRecalculateVisuals(const bool doRecalculation);

private:
	void Recalculate();

	bool m_doRecalculate = true;

	uint32_t m_maxWidth = 100;
	uint32_t m_charMax = 100;

	std::string m_contents;

	std::string m_characterName = "";
	sf::Text m_textVisual;
	sf::Text m_headerVisual;
	sf::RectangleShape m_colorBar;
	sf::RectangleShape m_headerBar;
	sf::RenderTexture m_renderTarget;
	sf::Sprite m_sprite;
};