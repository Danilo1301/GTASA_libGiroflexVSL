#pragma once

#include "pch.h"

#include "Vehicle.h"
#include "Point.h"

struct PointRotateData
{
    RwMatrix* matrix = NULL;
	float totalAngle = 0.0f;
    float roll = 0.0f;
};

struct RotateData
{
    std::map<Point*, PointRotateData*> pointsData;
};

class RotateObjectManager {
public:
    static std::map<Vehicle*, RotateData*> m_RotateData;

    static void ProcessVehicle(Vehicle* vehicle);

    static bool HasVehicle(Vehicle* vehicle);

    static void AddVehicle(Vehicle* vehicle);

    static PointRotateData* GetPointRotateData(Vehicle* vehicle, Point* point);

    static bool CheckIfPointsChanged(Vehicle* vehicle);

    static void ClearDataForVehicle(Vehicle* vehicle);

    static void ClearDataForAllVehicles();
};