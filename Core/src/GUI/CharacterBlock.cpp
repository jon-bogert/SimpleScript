#include "CharacterBlock.h"

#include "Style.h"

#include <algorithm>
#include <sstream>

void CharacterBlock::Start(uint32_t maxWidth)
{
	m_textVisual.setFillColor(sf::Color::White);
	m_textVisual.setFont(Style::Get().BodyFont());
	m_textVisual.setCharacterSize(Style::Get().BodyFontSize());

	m_headerVisual.setFillColor(sf::Color::White);
	m_headerVisual.setFont(Style::Get().BodyFont());
	m_headerVisual.setCharacterSize(Style::Get().BodyFontSize());

	m_headerBar.setFillColor({ 50, 50, 50, 255 });

    ChangeWidth(maxWidth);
}

void CharacterBlock::ChangeWidth(uint32_t maxWidth)
{
    sf::Text testText;
    testText.setFont(Style::Get().BodyFont());
    testText.setCharacterSize(Style::Get().BodyFontSize());

	uint32_t bodyWidth = maxWidth
		- Style::Get().ColorBarWidth()
		- Style::Get().BodyYOffset()
		- Style::Get().BodyPadding() * 4;

    while (testText.getLocalBounds().width <= bodyWidth)
    {
		std::string str = testText.getString();
        m_charMax = str.size();

		str.push_back('0');
		testText.setString(str);
    }

	size_t length = testText.getString().getSize();

	if (length > 1)
		m_charMax = testText.getString().getSize() - 1;
	else
		m_charMax = 1;

	m_maxWidth = maxWidth;
	Recalculate();
}

std::string CharacterBlock::GetText() const
{
    return m_contents;
}

void CharacterBlock::SetText(const std::string& text)
{
    m_contents = text;
    Recalculate();
}

void CharacterBlock::SetCharacter(Character* character)
{
    m_character = character;

	if (character == nullptr)
		return;

	m_headerVisual.setString(m_character->name);
}

void CharacterBlock::RenderTo(sf::RenderTexture* renderTarget)
{
	renderTarget->draw(m_sprite);
}

uint32_t CharacterBlock::GetHeight() const
{
	return m_height;
}

void CharacterBlock::Recalculate()
{
    std::stringstream stream(m_contents);
    std::string formatted;
    std::string word;
	std::string currLine;

    int counter = 0;

	while (std::getline(stream, word, ' '))
	{
		counter += word.length();
		if (counter + 1 > m_charMax)
		{
			formatted.append(currLine + "\n");
			counter = word.length();
			currLine = word;
			continue;
		}
		if (!currLine.empty())
		{
			currLine.push_back(' ');
			++counter;
		}
		currLine.append(word);
	}

	formatted.append(currLine);
	if (formatted.length() > 0 && formatted.back() == '\n')
		formatted.pop_back();

	m_textVisual.setString(formatted);

	m_height = m_textVisual.getLocalBounds().height + m_headerVisual.getLocalBounds().height;
	m_height = std::max(m_height, 1u);

	m_height += m_textVisual.getCharacterSize() * 0.5f
		+ m_headerVisual.getCharacterSize() * 0.5f
		+ Style::Get().BodyPadding() * 4.f;

	m_renderTarget.create(m_maxWidth, m_height);

	m_renderTarget.clear({ 20, 20, 20, 255 });

	sf::Vector2f cursor = { 0.f, 0.f };

	m_headerBar.setSize({
		(float)m_renderTarget.getSize().x,
		m_headerVisual.getLocalBounds().height + m_headerVisual.getCharacterSize() * 0.5f + Style::Get().BodyPadding() * 2.f
	});
	m_renderTarget.draw(m_headerBar);
	
	cursor.x = Style::Get().ColorBarWidth()
		+ Style::Get().BodyYOffset()
		+ Style::Get().BodyPadding();
	cursor.y = Style::Get().BodyPadding();

	m_headerVisual.setPosition(cursor);
	m_renderTarget.draw(m_headerVisual);

	cursor.y += m_headerVisual.getLocalBounds().height
		+ m_headerVisual.getCharacterSize() * 0.5f
		+ Style::Get().BodyPadding() * 2.f;

	m_textVisual.setPosition(cursor);
	m_renderTarget.draw(m_textVisual);

	m_colorBar.setSize({ (float)Style::Get().ColorBarWidth(), (float)m_height });
	sf::Color color = (m_character == nullptr) ? sf::Color(20, 20, 20, 255) : m_character->color;
	m_colorBar.setFillColor(color);
	m_renderTarget.draw(m_colorBar);

	m_renderTarget.display();
	m_sprite.setTexture(m_renderTarget.getTexture(), true);
}
