#pragma once

#include "../pch.h"

#include "INISection.h"

class INIFile {
public:
	std::vector<INISection*> sections;

	INISection* AddSection(std::string key);
	void Save(std::string path);
	void Destroy();

	bool Read(std::string path);

	std::vector<INISection*> GetSections(std::string name);
};