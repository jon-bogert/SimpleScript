#pragma once

#include "Character.h"
#include "GUI/RenderObject.h"

#include <filesystem>
#include <functional>
#include <deque>

class Project
{
public:
	void Start();
	void Load(const std::filesystem::path& filepath);
	
	void RenderTo(sf::RenderTexture* renderTarget);
	void Resize(uint32_t width, uint32_t height);

	void OnScroll(float amt, bool doRecalc = false);

	std::vector<RenderObject*> GetActiveBlocks();

private:
	std::string m_name = "Untitled";
	std::vector<std::unique_ptr<RenderObject>> m_blocks;
	
	size_t m_activeBlocksBegin = 0;
	size_t m_activeBlocksSize = 1;

	uint32_t m_viewportHeight = 0;

	size_t m_displayIndex = 0;
	float m_scrollOffset = 0;
	float m_scrollFactor = 15.f;
};