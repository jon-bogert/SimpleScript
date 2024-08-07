#pragma once

#include "Structs.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

struct Character
{
	std::string name;
	std::string notes;
	xe::Color color = xe::Color::Red;
};

class CharacterManifest
{
	CharacterManifest() {}
public:
	static CharacterManifest& Get() { static CharacterManifest instance; return instance; }
	~CharacterManifest() = default;
	CharacterManifest(const CharacterManifest& other) = delete;
	CharacterManifest(const CharacterManifest&& other) = delete;
	CharacterManifest operator=(const CharacterManifest& other) = delete;
	CharacterManifest operator=(const CharacterManifest&& other) = delete;


	const Character& operator[](const size_t i) const
	{
		return *m_characters[i];
	}

	const Character& operator[](const std::string& name) const
	{
		return *m_characters[m_lookup.at(name)];
	}

	size_t size() const
	{
		return m_characters.size();
	}

	size_t capacity() const
	{
		return m_characters.capacity();
	}

	void push_back(Character c)
	{
		m_lookup[c.name] = m_characters.size();
		m_characters.push_back(std::make_unique<Character>(c));
	}

	const Character& back() const
	{
		return *m_characters.back();
	}

	const Character& front() const
	{
		return *m_characters.front();
	}

	bool contains(const std::string& name)
	{
		return (m_lookup.find(name) != m_lookup.end());
	}

	void remove(size_t i)
	{
		m_lookup.erase(m_characters[i]->name);
		m_characters.erase(m_characters.begin() + i);
		while (i < m_characters.size())
		{
			m_lookup[m_characters[i++]->name]--;
		}
	}
	void remove(const std::string& name)
	{
		remove(m_lookup[name]);
	}

	void update(const size_t i, const Character& character)
	{
		Character& c = *m_characters[i];
		if (c.name != character.name)
		{
			m_lookup.erase(c.name);
			m_lookup[character.name] = i;
		}

		c = character;
	}

	void update(const std::string& old_name, const Character& character)
	{
		update(m_lookup[old_name], character);
	}

	size_t get_index(const std::string& name) const
	{
		return m_lookup.at(name);
	}

	bool empty() const
	{
		return m_characters.empty();
	}

	void clear()
	{
		m_characters.clear();
		m_lookup.clear();
	}

	void reserve(const size_t size)
	{
		m_characters.reserve(size);
	}

	void move_up(size_t index)
	{
		if (index <= 0)
			return;

		std::swap(m_characters[index], m_characters[index - 1]);
		std::swap(m_lookup[m_characters[index]->name], m_lookup[m_characters[index - 1]->name]);
	}

	void move_down(size_t index)
	{
		if (index >= m_characters.size() - 1)
			return;

		std::swap(m_characters[index], m_characters[index + 1]);
		std::swap(m_lookup[m_characters[index]->name], m_lookup[m_characters[index + 1]->name]);
	}

private:
	std::vector<std::unique_ptr<Character>> m_characters;
	std::unordered_map<std::string, size_t> m_lookup;
};