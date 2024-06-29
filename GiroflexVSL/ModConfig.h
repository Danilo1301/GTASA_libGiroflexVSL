#pragma once

#include "pch.h"

class ModConfig {
public:
	static bool IgnoreOldModVersionMessage;
	static bool TurnOnLightsWithSiren;

	static void MakePaths();
	static bool DirExists(std::string path);
	static bool FileExists(std::string path);
	static std::vector<std::string> GetDirectoriesName(std::string path);
	static void ConfigDeleteFile(std::string path);

	static std::string GetConfigFolder();
	static void CreateFolder(std::string path);

	static void WriteToFile(std::string path, Json::Value value);
	static Json::Value ReadFile(std::string path);
	
	static void Save();
	static void SavePatterns();
	static void SaveVehicles();
	static void SaveSettings();

	static void Load();
	static void LoadPatterns();
	static void LoadVehicles();
	static void LoadSettings();

	static std::string ReadVersionFile();
	static void ProcessVersionChanges_PreConfigLoad();
	static void ProcessVersionChanges_PostConfigLoad();

	static std::vector<std::string> FindRandomFileVariations(std::string src, int startNumber, std::string suffix);
};