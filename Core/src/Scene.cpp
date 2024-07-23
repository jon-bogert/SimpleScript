#include "Scene.h"

#include "Application.h"
#include "Debug.h"
#include "Project.h"
#include "Util.h"

#include <minidocx.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <yaml-cpp/yaml.h>

void Scene::AddBlock(TextBlock::Type type)
{
	TextBlock& tb = m_blocks.emplace_back();
	tb.type = type;
}

TextBlock& Scene::GetBlock(size_t index)
{
	return m_blocks[index];
}

void Scene::RemoveBlock(const size_t index)
{
	m_blocks.erase(m_blocks.begin() + index);
}

size_t Scene::NumberOfBlocks() const
{
	return m_blocks.size();
}

void Scene::Load(const std::filesystem::path& path, const SceneEntry& entryData)
{
	if (!std::filesystem::exists(path))
	{
		LOG("Scene could not find file: %s", path.c_str());
		return;
	}

	m_entryData = entryData;

	YAML::Node root = YAML::LoadFile(path.u8string());

	if (!root["contents"].IsDefined())
		return;

	for (const YAML::Node& blockEntry : root["contents"])
	{
		//data for error messages
		std::stringstream ss;
		ss << blockEntry;

		TextBlock block;
		if (!blockEntry["type"].IsDefined())
		{
			LOG("On Parse, content node missing 'type' field: %s", ss.str().c_str());
			continue;
		}

		std::string typeStr = blockEntry["type"].as<std::string>();
		if (typeStr == "Slug")
			block.type = TextBlock::Type::Slug;
		else if (typeStr == "Action")
			block.type = TextBlock::Type::Action;
		else if (typeStr == "Parenthetical")
			block.type = TextBlock::Type::Parenthetical;
		else if (typeStr == "Dialogue")
			block.type = TextBlock::Type::Dialogue;
		else
		{
			LOG("On Parse, unknown block type: %s", typeStr.c_str());
			continue;
		}

		switch (block.type)
		{
		case TextBlock::Type::Parenthetical:
		case TextBlock::Type::Dialogue:
			if (!blockEntry["character"].IsDefined())
			{
				LOG("On Parse, content node missing 'character' field: %s", ss.str().c_str());
				continue;
			}

			std::string character = blockEntry["character"].as<std::string>();
			Utility::AllCaps(character);
			if (Application::Get().GetActiveProject() == nullptr ||
				!Application::Get().GetActiveProject()->HasCharacter(character))
			{
				LOG("Warning: No Character found: %s", character.c_str());
			}
			block.character = character;
			break;
		}

		if (blockEntry["content"].IsDefined())
		{
			block.content = blockEntry["content"].as<std::string>();
		}
		
		m_blocks.push_back(block);
	}
}

void Scene::Export(const std::filesystem::path& path)
{
	docx::Document doc;

	std::string lastCharacter = "";
	bool wasLastBlockDialogue = false;
	uint32_t slugCount = 420;
	
	docx::Paragraph p;

	for (const TextBlock& block : m_blocks)
	{
		p = doc.AppendParagraph();
		if (block.type == TextBlock::Type::Parenthetical ||
			block.type == TextBlock::Type::Dialogue)
		{
			if (!wasLastBlockDialogue || block.character != lastCharacter)
			{
				//Empty Line
				p.AppendRun("");
				p.SetFont("CourierPrime");
				p.SetFontSize(12.);
				
				p = doc.AppendParagraph();

				if (block.character == lastCharacter)
					p.AppendRun("\t\t\t\t\t" + block.character + " (CONT'D)");
				else
					p.AppendRun("\t\t\t\t\t" + block.character);

				p.SetFont("CourierPrime");
				p.SetFontSize(12.);
				p = doc.AppendParagraph();
			}

			if (block.type == TextBlock::Type::Parenthetical)
			{
				p.AppendRun("\t\t\t\t(");
				p.AppendRun(block.content);
				p.AppendRun(")");
				p.SetFont("CourierPrime");
				p.SetFontSize(12.);
			}
			else // Dialogue
			{
				std::vector<std::string> formatted = Utility::DialogueLineBreaks(block.content);
				for (const std::string& line : formatted)
				{
					p.AppendRun("\t\t\t" + line);
					p.SetFont("CourierPrime");
					p.SetFontSize(12.);
					if (line != formatted.back())
						p = doc.AppendParagraph();
				}
			}

			lastCharacter = block.character;
			wasLastBlockDialogue = true;
			continue;
		}

		wasLastBlockDialogue = false;

		//Empty Line
		p.AppendRun("");
		p.SetFont("CourierPrime");
		p.SetFontSize(12.);

		p = doc.AppendParagraph();

		if (block.type == TextBlock::Type::Slug)
		{
			p.AppendRun(Utility::SlugFormat(slugCount++, block.content));
			p.SetFont("CourierPrime");
			p.SetFontSize(12.);
			p.SetFontStyle(docx::Run::Bold);
			continue;
		}

		//Action
		std::vector<std::string> formatted = Utility::ActionLineBreaks(block.content);
		for (const std::string& line : formatted)
		{
			p.AppendRun("\t" + line);
			p.SetFont("CourierPrime");
			p.SetFontSize(12.);
			if (line != formatted.back())
				p = doc.AppendParagraph();
		}
	}

	docx::Section sec = doc.FirstSection();
	sec.SetPageMargin(
		docx::Inch2Twip(1.),
		docx::Inch2Twip(1.), 
		docx::Inch2Twip(1.), 
		docx::Inch2Twip(1.));

	doc.Save(path.u8string());
}
