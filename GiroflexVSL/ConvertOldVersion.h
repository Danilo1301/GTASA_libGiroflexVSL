#pragma once

#include "pch.h"

#include "LightGroup.h"
#include "iniconfig/INISection.h"

class ConvertOldVersion {
public:
    static void MakePaths();

    static void Load();
    static void LoadPatterns();
	static void LoadVehicles();

	static void LightGroupFromINISection(LightGroup* lightGroup, INISection* section);
};