#include "LightGroupDatas.h"

int LightGroupDatas::m_ChangePatternTime = 3000;
std::vector<LightGroupData*> LightGroupDatas::m_LightGroupDatas;

bool LightGroupDatas::HasLightGroupData(LightGroup* lightGroup, int hVehicle)
{
	return GetLightGroupData(lightGroup, hVehicle) != NULL;
}

void LightGroupDatas::AddLightGroupData(LightGroupData* lightGroupData)
{
	m_LightGroupDatas.push_back(lightGroupData);
}

LightGroupData* LightGroupDatas::GetLightGroupData(LightGroup* lightGroup, int hVehicle)
{
	for (auto lightGroupData : m_LightGroupDatas)
	{
		if (lightGroupData->lightGroup == lightGroup && lightGroupData->hVehicle == hVehicle) return lightGroupData;
	}
	return NULL;
}