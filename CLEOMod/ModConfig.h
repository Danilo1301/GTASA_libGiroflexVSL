#pragma once

#include "pch.h"

class ModConfig {
public:
	static void MakePaths();

	static std::string GetConfigFolder();
	static void CreateFolder(std::string path);
	static void Save();
	static void Load();
};