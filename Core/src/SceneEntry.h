#pragma once

#include "Util.h"

#include <filesystem>

// Contains information used to know which scene files can be loaded
struct SceneEntry
{
	uint32_t act = 0;
	uint32_t scene = 0;
	std::string name = "Untitled";

	std::filesystem::path GetFilename()
	{
		return std::filesystem::path(L"SCENE_" + Utility::TwoDigUInt(act) + L"_" + Utility::TwoDigUInt(scene));
	}
};