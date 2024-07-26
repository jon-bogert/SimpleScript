#pragma once

#include "GUI/Style.h"

#include <SFML/Graphics/RenderTexture.hpp>

class RenderObject
{
public:
	virtual void RenderTo(sf::RenderTexture* renderTarget) = 0;
	virtual void SetYPosition(const float y) = 0;
	virtual void Recalculate() = 0;

	RenderObject() = default;
	virtual ~RenderObject() {};

	uint32_t GetHeight() const
	{
		return m_height;
	}

	static void ChangeWidth(uint32_t maxWidth)
	{
		sf::Text testText;
		testText.setFont(Style::Get().BodyFont());
		testText.setCharacterSize(Style::Get().BodyFontSize());

		Style& style = Style::Get();

		int64_t bodyWidth = maxWidth;
		bodyWidth -= (int64_t)Style::Get().ColorBarWidth()
			+ Style::Get().BodyYOffset()
			+ Style::Get().BodyPadding() * 2;

		while (testText.getLocalBounds().width < bodyWidth)
		{
			std::string str = testText.getString();
			s_boundsInfo.maxChar = str.size();

			str.push_back('0');
			testText.setString(str);
		}

		size_t length = testText.getString().getSize();

		if (length > 1)
			s_boundsInfo.maxChar = testText.getString().getSize() - 1;
		else
			s_boundsInfo.maxChar = 1;

		s_boundsInfo.maxWidth = maxWidth;
		//Recalculate();
	}

protected:
	struct BoundsInfo
	{
		uint32_t maxWidth = 100;
		uint32_t maxChar = 100;
	};

	inline static BoundsInfo s_boundsInfo{};

	uint32_t m_height = 1;
};