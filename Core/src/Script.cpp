#include "Script.h"

#include "Application.h"
#include "Debug.h"
#include "FileBrowser.h"
#include "Structs.h"
#include "Util.h"

#include <minidocx.hpp>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>

void Script::AddBlock(TextBlock::Type type, size_t index)
{
	if (index == SIZE_MAX || index >= m_blocks.size() - 1)
	{
		TextBlock& tb = m_blocks.emplace_back();
		tb.type = type;
		return;
	}
	TextBlock& tb = *m_blocks.emplace(m_blocks.begin() + index + 1);
	tb.type = type;
}

TextBlock& Script::GetBlock(size_t index)
{
	return m_blocks[index];
}

void Script::RemoveBlock(const size_t index)
{
	m_blocks.erase(m_blocks.begin() + index);
}

size_t Script::NumberOfBlocks() const
{
	return m_blocks.size();
}

bool Script::MoveUp(size_t index)
{
	if (index == 0)
		return false;

	std::swap(m_blocks[index], m_blocks[index - 1]);
	return true;
}

bool Script::MoveDown(size_t index)
{
	if (index >= m_blocks.size() - 1)
		return false;

	std::swap(m_blocks[index], m_blocks[index + 1]);
	return true;
}

void Script::Load(const std::filesystem::path path)
{
	if (!std::filesystem::exists(path))
	{
		LOG("Scene could not find file: %s", path.c_str());
		return;
	}

	Application& app = Application::Get();
	auto found = std::find(app.recentFiles.begin(), app.recentFiles.end(), path);
	if (found != app.recentFiles.end())
	{
		app.recentFiles.erase(found);
		app.recentFiles.insert(app.recentFiles.begin(), path);
	}
	else
	{
		if (app.recentFiles.size() >= app.RECENT_MAX)
		{
			app.recentFiles.pop_back();
		}
		app.recentFiles.insert(app.recentFiles.begin(), path);
	}

	New();

	CharacterManifest& characters = CharacterManifest::Get();
	m_filepath = path;

	YAML::Node root = YAML::LoadFile(path.u8string());

	if (root["name"].IsDefined())
	{
		m_name = root["name"].as<std::string>();
	}

	if (root["characters"].IsDefined())
	{

		for (const YAML::Node charEntry : root["characters"])
		{
			if (!charEntry["name"].IsDefined())
			{
				LOG("Bad Character entry, no name field");
				continue;
			}
			Character character;

			character.name = charEntry["name"].as<std::string>();
			Utility::AllCaps(character.name);

			if (charEntry["color"].IsDefined() && charEntry["color"].size() >= 3)
			{
				xe::Color8 color;
				color.r = charEntry["color"][0].as<uint8_t>();
				color.g = charEntry["color"][1].as<uint8_t>();
				color.b = charEntry["color"][2].as<uint8_t>();
				character.color = color;
			}

			if (charEntry["notes"].IsDefined())
			{
				character.notes = charEntry["notes"].as<std::string>();
			}

			characters.push_back(character);
		}
	}

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
		else if (typeStr == "Note")
			block.type = TextBlock::Type::Note;
		else
		{
			LOG("On Parse, unknown block type: %s", typeStr.c_str());
			continue;
		}

		CharacterManifest& characterManiftest = CharacterManifest::Get();

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
			if (!characterManiftest.contains(character))
			{
				LOG("Warning: No Character found: %s", character.c_str());
			}
			block.character = character;
			break;
		}

		if (blockEntry["text"].IsDefined())
		{
			block.content = blockEntry["text"].as<std::string>();
		}
		
		m_blocks.push_back(block);
	}
}

void Script::Export()
{
	xe::FileBrowser browser;
	browser.PushFileType(L"*.docx", L"Word Document");
	browser.SetStartPath(Application::Get().lastExport);
	std::wstring pathStr = browser.SaveFile();
	if (pathStr.empty())
		return;

	std::filesystem::path path = pathStr;
	Application::Get().lastExport = path.parent_path();

	docx::Document doc;

	std::string lastCharacter = "";
	bool wasLastBlockDialogue = false;
	uint32_t slugCount = 1;
	
	docx::Paragraph p;

	for (const TextBlock& block : m_blocks)
	{
		if (block.type == TextBlock::Type::Note)
			continue;

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

void Script::Save(bool doForceDialogue)
{
	Application& app = Application::Get();
	if (m_filepath.empty() || doForceDialogue)
	{
		std::wstring path = SaveDialogue();
		if (path.empty())
			return;

		m_filepath = path;
		if (app.recentFiles.size() >= app.RECENT_MAX)
		{
			app.recentFiles.pop_back();
			app.recentFiles.insert(app.recentFiles.begin(), path);
		}

		auto found = std::find(app.recentFiles.begin(), app.recentFiles.end(), path);
		if (found != app.recentFiles.end())
		{
			app.recentFiles.erase(found);
			app.recentFiles.insert(app.recentFiles.begin(), path);
		}
		else
		{
			if (app.recentFiles.size() >= app.RECENT_MAX)
			{
				app.recentFiles.pop_back();
			}
			app.recentFiles.insert(app.recentFiles.begin(), path);
		}
	}

	std::ofstream file;
	file.open(m_filepath + L"\\_project.yaml");

	YAML::Node root;
	root["name"] = m_name;
	CharacterManifest& manifest = CharacterManifest::Get();
	for (size_t i = 0; i < manifest.size(); ++i)
	{
		const Character& character = manifest[i];
		YAML::Node charEntry;
		charEntry["name"] = character.name;
		charEntry["color"].push_back((int)character.color.As8bit().r);
		charEntry["color"].push_back((int)character.color.As8bit().g);
		charEntry["color"].push_back((int)character.color.As8bit().b);
		if (!character.notes.empty())
			charEntry["notes"] = character.notes;
		
		root["characters"].push_back(charEntry);
	}

	file << root;
	file.close();

	if (m_blocks.empty())
	{
		app.isSaved = true;
		return;
	}

	root = YAML::Node();
	std::string filename{};

	size_t blockCount = 0;
	for (const TextBlock& block : m_blocks)
	{

		YAML::Node blockEntry;
		switch (block.type)
		{
		case TextBlock::Slug:
			blockEntry["type"] = "Slug";
			if (!filename.empty()) // Save if not first
			{
				file.open(std::filesystem::path(m_filepath) / (filename + ".yaml"));
				file << root;
				file.close();
				root = YAML::Node(); // Clear;
			}
			filename = Utility::SlugFileFormat(blockCount++, block.content);
			break;
		case TextBlock::Action:
			blockEntry["type"] = "Action";
			break;
		case TextBlock::Parenthetical:
			blockEntry["type"] = "Parenthetical";
			blockEntry["character"] = block.character;
			break;
		case TextBlock::Dialogue:
			blockEntry["type"] = "Dialogue";
			blockEntry["character"] = block.character;
			break;
		case TextBlock::Note:
			blockEntry["type"] = "Note";
			break;
		}

		blockEntry["text"] = block.content;

		root["contents"].push_back(blockEntry);
	}

	file.open(std::filesystem::path(m_filepath) / (filename + ".yaml"));
	file << root;
	file.close();

	app.isSaved = true;
}

void Script::New()
{
	CharacterManifest& characters = CharacterManifest::Get();
	characters.clear();
	m_blocks.clear();
	m_name = "Untitled";
	m_filepath = L"";
	Application::Get().editIndex = SIZE_MAX;
}

void Script::OpenDialogue()
{
	xe::FileBrowser browser;
	browser.PushFileType(L"*.yaml", L"YAML File");
	browser.SetStartPath(Application::Get().lastOpen);
	std::wstring path = browser.GetFile();
	if (path.empty())
		return;

	Application::Get().lastOpen = std::filesystem::path(path).parent_path();
	Load(path);
}

std::wstring Script::SaveDialogue()
{
	xe::FileBrowser browser;
	browser.SetStartPath(Application::Get().lastSave);
	std::filesystem::path path = browser.LoadFolder();
	if (!path.empty())
	{
		Application::Get().lastSave = path;
	}
	return path;
}
