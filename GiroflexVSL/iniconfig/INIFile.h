#pragma once

#include "../pch.h"

#include "INISection.h"

/*
[1.0.0] 26/05/24

[1.1.0] 14/06/24
Added GetBoolFromInt and AddIntFromBool to INISection
Now it recognizes empty lines
It recognizes lines starting with ; as a comment
Updated GetCVectorWithDefaultValue and GetCVector

[1.1.1] 20/06/24
Added GetCVector2D and GetCRGBA
*/

class INIFile {
public:
	std::vector<INISection*> sections;

	INISection* AddSection(std::string key);
	void Save(std::string path);
	void Destroy();

	bool Read(std::string path);

	std::vector<INISection*> GetSections(std::string name);
};