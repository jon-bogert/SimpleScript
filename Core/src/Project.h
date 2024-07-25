#pragma once

#include "Character.h"
#include "GUI/RenderObject.h"

#include <filesystem>

class Project
{
public:
	void Start();
	void Load(const std::filesystem::path& filepath);
	
	void RenderTo(sf::RenderTexture* renderTarget);
	void Resize(uint32_t width, uint32_t height);

private:
	std::string m_name = "Untitled";
	std::vector<std::unique_ptr<RenderObject>> m_blocks;
};