#pragma once

#include "pch.h"

#include "LightGroupData.h"

class Vehicle {
public:
	int hVehicle;
	int modelId;
	CVector position = CVector(0, 0, 0);
	CVector velocity = CVector(0, 0, 0);

	CVehicle* pVehicle = NULL;
	bool gameSirenState = false;
	void* pDriver = NULL;

	bool canBeRemoved = false;

	bool prevLightsState = false;
	bool prevSirenState = false;
	//bool lightsOn = false;
	//bool lightsPaused = false;

	Vehicle(int hVehicle, int modelId);
	~Vehicle();

	void Destroy();
	void Update(int dt);

	std::vector<LightGroupData*> GetLightGroupsData();

	void SetGiroflexEnabled(bool enabled);
};