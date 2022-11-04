#include "Vehicle.h"
#include "LightGroupDatas.h"
#include "ModelInfos.h"

Vehicle::Vehicle(int hVehicle, int modelId)
{
	this->hVehicle = hVehicle;
	this->modelId = modelId;
}

void Vehicle::Destroy()
{

}

/*

std::vector<LightGroupData*> Vehicle::GetLightGroupsData()
{
	std::cout << "GetLightGroupsData" << std::endl;

	std::vector<LightGroupData*> lightGroupDataList;

	if (!ModelInfos::HasModelInfo(this->modelId)) return lightGroupDataList;

	auto modelInfo = ModelInfos::GetModelInfo(this->modelId);

	for (auto lightGroup : modelInfo->lightGroups)
	{
		if (!LightGroupDatas::HasLightGroupData(lightGroup)) continue;

		lightGroupDataList.push_back(LightGroupDatas::m_LightGroupDatas[lightGroup]);
	}

	return lightGroupDataList;
}
*/

void Vehicle::SetGiroflexEnabled(bool enabled)
{
	lightsPaused = !enabled;
	lightsOn = enabled;

	/*
	auto lightGroupDataList = GetLightGroupsData();

	std::cout << "lightGroupDataList " << lightGroupDataList.size() << std::endl;

	for (auto lightGroupData : lightGroupDataList)
	{
		lightGroupData->stepLoop->DontChangeSteps = !enabled;
	}
	*/

}