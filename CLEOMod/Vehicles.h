#pragma once

#include "pch.h"
#include "Vehicle.h"

class Vehicles {
public:
	static std::map<int, Vehicle*> m_Vehicles;

	static void TryCreateVehicle(int hVehicle, int modelId);
	static bool HasVehicleHandle(int hVehicle);
	static Vehicle* GetVehicleByHandle(int hVehicle);
	static void RemoveVehicle(int hVehicle);
	static void CheckStreamedOutVehicles();

	static void UpdateVehiclesPattern(int dt);
};