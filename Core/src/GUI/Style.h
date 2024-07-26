#pragma once

#include <SFML/Graphics.hpp>

class Style final
{
    Style();

public:
    static Style& Get() { static Style instance; return instance; }
    ~Style() = default;
    Style(const Style& other) = delete;
    Style(const Style&& other) = delete;
    Style operator=(const Style& other) = delete;
    Style operator=(const Style&& other) = delete;

    const sf::Font& BodyFont() const { return m_bodyFont; }
    uint32_t BodyFontSize() const { return m_bodyFontSize; }
    uint32_t HeaderFontSize() const { return m_headerFontSize; }
    uint32_t ColorBarWidth() const { return m_colorBarWidth; }
    uint32_t BodyYOffset() const { return m_bodyYOffset; }
    uint32_t BodyPadding() const { return m_bodyPadding; }
    uint32_t BodyBackgroundMaxLevel() const { return m_bodyBackgroundMaxLevel; }
    uint32_t BlockGap() const { return m_blockGap; }

private:

    //TextBox
    sf::Font m_bodyFont;
    uint32_t m_bodyFontSize = 18;
    uint32_t m_headerFontSize = 30;
    uint32_t m_colorBarWidth = 7;
    uint32_t m_bodyYOffset = 5;
    uint32_t m_bodyPadding = 10;
    uint32_t m_bodyBackgroundMaxLevel = 30;
    uint32_t m_blockGap = 10;
};
