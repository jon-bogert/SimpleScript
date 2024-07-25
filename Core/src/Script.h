#pragma once

#include "TextBlock.h"
#include "Character.h"

#include <filesystem>
#include <vector>

namespace sf
{
	class RenderTexture;
}

class Script
{
public:
	void AddBlock(TextBlock::Type type);
	TextBlock& GetBlock(size_t index);
	void RemoveBlock(const size_t index);
	size_t NumberOfBlocks() const;

	void Load(const std::filesystem::path& path);
	void Export(const std::filesystem::path& path);

	std::string GetName() const { return m_name; }
	void SetName(const std::string& name ) { m_name = name; }

private:
	std::vector<TextBlock> m_blocks;
	std::string m_name = "Untitled";
};

