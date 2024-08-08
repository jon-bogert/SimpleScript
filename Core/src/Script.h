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
	void AddBlock(TextBlock::Type type, size_t index = SIZE_MAX);
	TextBlock& GetBlock(size_t index);
	void RemoveBlock(const size_t index);
	size_t NumberOfBlocks() const;
	bool MoveUp(size_t index);
	bool MoveDown(size_t index);

	void Load(const std::filesystem::path path); // copy on purpose (possible move of stored path)
	void Export();
	void Save(bool doForceDialogue = false);
	void New();

	std::string GetName() const { return m_name; }
	void SetName(const std::string& name ) { m_name = name; }

	void OpenDialogue();

private:
	std::wstring SaveDialogue();

	std::vector<TextBlock> m_blocks;
	std::string m_name = "Untitled";
	std::wstring m_filepath = L"";
};

