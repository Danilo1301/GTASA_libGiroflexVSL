#pragma once

#include "pch.h"
#include "LightGroup.h"

class ModelInfo {
public:
	int modelId;
	std::vector<LightGroup*> lightGroups;

	ModelInfo(int modelId)
	{
		this->modelId = modelId;
	}

	LightGroup* AddLightGroup(LightGroup* lightGroup)
	{
		lightGroups.push_back(lightGroup);
		return lightGroup;
	}
};