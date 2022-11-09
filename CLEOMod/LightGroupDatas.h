#pragma once

#include "pch.h"

#include "LightGroup.h"
#include "LightGroupData.h"
#include "Vehicle.h"

class LightGroupDatas {
public:
	static int m_ChangePatternTime;
	static std::vector<LightGroupData*> m_LightGroupDatas;

	static bool HasLightGroupData(LightGroup* lightGroup, int hVehicle);
	static void AddLightGroupData(LightGroupData* lightGroupData);
	static LightGroupData* GetLightGroupData(LightGroup* lightGroup, int hVehicle);

	static void RemoveLightGroupDataFromVehicle(int hVehicle);
	static void RemoveLightGroupData(LightGroupData* lightGroupData);
	static void DeleteLightGroupRerefences(LightGroup* lightGroup);
};