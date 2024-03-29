#pragma once

#include "pch.h"
#include "LightGroup.h"

class ModelInfo {
public:
	int modelId;
	std::vector<LightGroup*> lightGroups;

	ModelInfo(int modelId);

	LightGroup* CreateLightGroup();
	LightGroup* AddLightGroup(LightGroup* lightGroup);
	void RemoveLightGroup(LightGroup* lightGroup);
	LightGroup* DuplicateLightGroup(LightGroup* lightGroup);
};