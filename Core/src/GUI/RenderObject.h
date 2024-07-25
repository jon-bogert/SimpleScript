#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

class RenderObject
{
public:
	virtual void RenderTo(sf::RenderTexture* renderTarget) = 0;

	RenderObject() = default;
	virtual ~RenderObject() {};

	void SetOffset(uint32_t& offset)
	{
		m_offset = offset;
		offset += m_height;
	}

	uint32_t GetHeight() const
	{
		return m_height;
	}


	virtual void ChangeWidth(uint32_t maxWidth) {};

protected:
	uint32_t m_offset = 0;
	uint32_t m_height = 1;
};