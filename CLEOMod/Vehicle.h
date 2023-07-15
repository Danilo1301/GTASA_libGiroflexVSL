#pragma once

#include "pch.h"

#include "LightGroupData.h"

class Vehicle {
public:
	int hVehicle;
	int modelId;
	CVector position = CVector(0, 0, 0);
	CVector velocity = CVector(0, 0, 0);

	bool canBeRemoved = false;

	bool prevLightsState = false;
	//bool lightsOn = false;
	//bool lightsPaused = false;

	Vehicle(int hVehicle, int modelId);

	void Destroy();
	void Update(int dt);

	std::vector<LightGroupData*> GetLightGroupsData();

	void SetGiroflexEnabled(bool enabled);
};