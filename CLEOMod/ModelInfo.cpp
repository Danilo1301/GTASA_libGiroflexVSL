#include "ModelInfo.h"
#include "LightGroupDatas.h"

ModelInfo::ModelInfo(int modelId)
{
	this->modelId = modelId;
};

LightGroup* ModelInfo::AddLightGroup(LightGroup* lightGroup)
{
	lightGroups.push_back(lightGroup);
	return lightGroup;
}

void ModelInfo::RemoveLightGroup(LightGroup* lightGroup)
{
	LightGroupDatas::DeleteLightGroupRerefences(lightGroup);

	auto it = std::find(lightGroups.begin(), lightGroups.end(), lightGroup);
	if (it == lightGroups.end()) return;
	lightGroups.erase(it);
	lightGroup->Destroy();
	delete lightGroup;
}

LightGroup* ModelInfo::DuplicateLightGroup(LightGroup* lightGroup)
{
	LightGroup* newLightGroup = new LightGroup();
	newLightGroup->FromINISection(lightGroup->ToINISection());

	AddLightGroup(newLightGroup);

	newLightGroup->MakeLightGroup();

	return newLightGroup;
}