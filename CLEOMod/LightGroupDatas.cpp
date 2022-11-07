#include "LightGroupDatas.h"
#include "Log.h"
#include "Vehicles.h"

int LightGroupDatas::m_ChangePatternTime = 5000;
std::vector<LightGroupData*> LightGroupDatas::m_LightGroupDatas;

bool LightGroupDatas::HasLightGroupData(LightGroup* lightGroup, int hVehicle)
{
	return GetLightGroupData(lightGroup, hVehicle) != NULL;
}

void LightGroupDatas::AddLightGroupData(LightGroupData* lightGroupData)
{
	Log::file << "LightGroupDatas: AddLightGroupData" << std::endl;

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

void LightGroupDatas::ResetPatterns(int hVehicle)
{
	Log::file << "LightGroupDatas: ResetPatterns" << std::endl;

	std::vector<LightGroupData*> toRemove;
	for (auto lightGroupData : m_LightGroupDatas)
	{
		if (lightGroupData->hVehicle == hVehicle)
			toRemove.push_back(lightGroupData);
	}

	for (auto lightGroupData : toRemove)
	{
		RemoveLightGroupData(lightGroupData);
	}
}
void LightGroupDatas::RemoveLightGroupData(LightGroupData* lightGroupData)
{
	Log::file << "LightGroupDatas: RemoveLightGroupData" << std::endl;

	auto it = std::find(m_LightGroupDatas.begin(), m_LightGroupDatas.end(), lightGroupData);
	if (it == m_LightGroupDatas.end()) return;
	m_LightGroupDatas.erase(it);
	delete lightGroupData;
}

void LightGroupDatas::DeleteLightGroupRerefences(LightGroup* lightGroup)
{
	Log::file << "LightGroupDatas: Deleting LightGroup references..." << std::endl;

	std::vector<int> vehicles;

	for (auto lightGroupData : m_LightGroupDatas)
	{
		if (lightGroupData->lightGroup == lightGroup)
		{
			vehicles.push_back(lightGroupData->hVehicle);
		}
	}

	for (auto hVehicle : vehicles)
	{
		ResetPatterns(hVehicle);
	}
}