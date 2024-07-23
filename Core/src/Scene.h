#pragma once

#include "SceneEntry.h"
#include "TextBlock.h"

#include <filesystem>
#include <vector>

namespace sf
{
	class RenderTexture;
}

class Scene
{
public:
	void AddBlock(TextBlock::Type type);
	TextBlock& GetBlock(size_t index);
	void RemoveBlock(const size_t index);
	size_t NumberOfBlocks() const;

	void Load(const std::filesystem::path& path, const SceneEntry& entryData);
	void Export(const std::filesystem::path& path);

private:
	SceneEntry m_entryData{};

	std::vector<TextBlock> m_blocks;
};

