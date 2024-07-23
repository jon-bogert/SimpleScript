#include "Project.h"

#include "Character.h"
#include "Scene.h"
#include "Debug.h"

#include <yaml-cpp/yaml.h>

#include <sstream>
#include <string>

void Project::AddCharacter(const Character& character)
{
	if (HasCharacter(character.name))
		return;

	m_characterLookup[character.name] = m_characters.size();
	m_characters.push_back(character);
}

void Project::UpdateCharacter(const std::string& name, const Character& info)
{
	if (!HasCharacter(name))
		return;

	size_t index = m_characterLookup[name];

	//Check name change
	if (name != info.name)
	{
		m_characterLookup.erase(name);
		m_characterLookup[info.name] = index;
	}

	m_characters[index] = info;
}

bool Project::HasCharacter(const std::string& name) const
{
	return (m_characterLookup.find(name) != m_characterLookup.end());
}

Character Project::GetCharacter(const std::string& name) const
{
	if (!HasCharacter(name))
		return Character();

	return m_characters[m_characterLookup.at(name)];
}

bool Project::RemoveCharacter(const std::string& name)
{
	if (!HasCharacter(name))
		return false;

	size_t index = m_characterLookup[name];
	m_characters.erase(m_characters.begin() + index);
	m_characterLookup.erase(name);

	// fix lookup indices
	while (index < m_characters.size())
	{
		m_characterLookup[m_characters[index].name]--; // decrement the index
		++index;
	}
	return true;
}

void Project::MoveCharacter(const size_t index, const bool isForward)
{
	if (isForward)
	{
		if (index >= m_characters.size() - 1)
			return;

		// Adjust lookup values
		m_characterLookup[m_characters[index].name]++;
		m_characterLookup[m_characters[index + 1].name]--;

		//Do the swap
		std::swap(m_characters[index], m_characters[index + 1]);
		return;
	}

	// Backward
	if (index <= 0)
		return;

	// Adjust lookup values
	m_characterLookup[m_characters[index].name]--;
	m_characterLookup[m_characters[index - 1].name]++;

	//Do the swap
	std::swap(m_characters[index], m_characters[index - 1]);
}

const std::vector<Character>& Project::GetCharacters() const
{
	return m_characters;
}

size_t Project::NumberOfScenes() const
{
	return m_scenes.size();
}

bool Project::OpenFromDirectory(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		LOG("Project directory not found at: %s", path.u8string().c_str());
		return false;
	}

	m_directory = path;

	std::filesystem::path charPath = m_directory / L"characters.yaml";
	if (std::filesystem::exists(charPath))
	{
		LOG("TODO - Load characters");
	}
	
	//Find Scene files
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_regular_file())
			continue;

		std::wstringstream sstream;
		sstream << entry.path().filename().wstring();

		// Check Filename

		std::wstring cell;
		std::getline(sstream, cell, L'_');
		if (cell != L"SCENE") // Confirm Scene filename
			continue;

		SceneEntry sceneEntry{};

		// Get Act Number
		std::getline(sstream, cell, L'_');
		try
		{
			uint32_t i = std::stoul(cell);
			sceneEntry.act = i;
		}
		catch (std::exception)
		{
			LOG("Could not parse filename act: %s", entry.path().filename().u8string().c_str());
			continue;
		}

		// Get Scene Number
		std::getline(sstream, cell, L'.');
		try
		{
			uint32_t i = std::stoul(cell);
			sceneEntry.scene = i;
		}
		catch (std::exception)
		{
			LOG("Could not parse filename scene: %s", entry.path().filename().u8string().c_str());
			continue;
		}

		// Find Name from file contents
		YAML::Node root = YAML::LoadFile(entry.path().string());
		if (!root["name"].IsDefined())
		{
			sceneEntry.name = root["name"].as<std::string>();
		}

		//Store info for loading later
		m_scenes.push_back(sceneEntry);
	}

	return true;
}
