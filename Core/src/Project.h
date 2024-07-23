#pragma once

#include "Character.h"
#include "Scene.h"
#include "Util.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class Project
{
public:
	void AddCharacter(const Character& character);
	void UpdateCharacter(const std::string& name, const Character& info);
	bool HasCharacter(const std::string& name) const;
	Character GetCharacter(const std::string& name) const;
	bool RemoveCharacter(const std::string& name);
	void MoveCharacter(const size_t index, const bool isForward = true);
	const std::vector<Character>& GetCharacters() const;

	Scene& GetActiveScene() { return m_activeScene; }
	const Scene& GetActiveScene() const { return m_activeScene; }
	size_t Project::NumberOfScenes() const;

	void SetActiveScene(const size_t index);
	void SaveActiveScene();


	bool OpenFromDirectory(const std::filesystem::path& path);

private:
	std::string m_name = "Untitled";
	std::string m_description = "";

	std::vector<SceneEntry> m_scenes;
	Scene m_activeScene;

	std::vector<Character> m_characters;
	std::unordered_map<std::string, size_t> m_characterLookup;

	std::filesystem::path m_directory;
};

