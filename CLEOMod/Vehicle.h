#pragma once

#include "pch.h"

#include "LightGroupData.h"

class Vehicle {
public:
	int hVehicle;
	int modelId;
	CVector position = CVector(0, 0, 0);

	bool lightsOn = true;
	bool lightsPaused = false;

	Vehicle(int hVehicle, int modelId);

	void Destroy();

	//std::vector<LightGroupData*> GetLightGroupsData();

	void SetGiroflexEnabled(bool enabled);
};