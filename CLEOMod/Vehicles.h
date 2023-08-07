#pragma once

#include "pch.h"
#include "Vehicle.h"

struct RenderCorona {
	int id = 25;
	int car = 0;
	void* pVehicle = 0;
	CVector offset = CVector(0, 0, 0);
	CRGBA color = CRGBA(255, 0, 0);
	float radius = 1.0f;

	bool renderShadow = false;
	float shadowIntensity = 1.0f;
	float shadowSize = 5.0f;
	float shadowOffsetX = 0.0f;
	float shadowOffsetY = 0.0f;
	int shadowTexture = 3;
	float shadowRotation = 0.0f;

	bool renderPointLight = false;
	float pointLightIntensity = 1.0f;
	float pointLightDistance = 60.0f;

	float nearClip = 0.0f;

	bool useFlare = false;
	float flareIntensity = 1.00f;
	float flareDistance = 1.00f;
	int flareTexture = 0;

	int coronaTexture = 0;
};

class Vehicles {
public:
	
	static std::map<int, Vehicle*> m_Vehicles;
	static std::vector<RenderCorona> m_CoronasToRender;
	static std::vector<int> m_NewVehiclesRef;

	static void TryCreateVehicle(int hVehicle, int modelId);
	static bool HasVehicleHandle(int hVehicle);
	static Vehicle* GetVehicleByHandle(int hVehicle);
	static Vehicle* GetVehicleByVecIndex(int index);
	static void RemoveVehicle(int hVehicle);
	static void CheckStreamedOutVehicles();

	static void Update(int dt);

	static void AddCoronaToRender(RenderCorona corona);

	static void TryFindNewVehicles();
};