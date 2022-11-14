#pragma once

#include "pch.h"
#include "Vehicle.h"

struct RenderCorona {
	int id = 25;
	int car = 0;
	CVector offset = CVector(0, 0, 0);
	CRGBA color = CRGBA(255, 0, 0);
	float radius = 1.0f;

	bool renderShadow = true;
	float shadowIntensity = 1.0f;
	float shadowSize = 5.0f;

	bool renderPointLight = true;
	float pointLightIntensity = 1.0f;
	float pointLightDistance = 60.0f;

	float nearClip = 0.2f;
};

class Vehicles {
public:
	static int hPlayerVehicle;
	static std::map<int, Vehicle*> m_Vehicles;
	static std::vector<RenderCorona> m_CoronasToRender;

	static void TryCreateVehicle(int hVehicle, int modelId);
	static bool HasVehicleHandle(int hVehicle);
	static Vehicle* GetVehicleByHandle(int hVehicle);
	static Vehicle* GetVehicleByVecIndex(int index);
	static void RemoveVehicle(int hVehicle);
	static void CheckStreamedOutVehicles();

	static void Update(int dt);

	static bool IsPlayerInAnyVehicle();
	static Vehicle* GetPlayerVehicle();
};