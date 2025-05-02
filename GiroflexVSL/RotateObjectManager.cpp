#include "RotateObjectManager.h"

#include "ModelInfos.h"

std::map<Vehicle*, RotateData*> RotateObjectManager::m_RotateData;

void RotateObjectManager::ProcessVehicle(Vehicle* vehicle)
{
    //Log::Level(LOG_LEVEL::LOG_BOTH) << "RotateObjectManager: Process vehicle " << vehicle << std::endl;

    if(HasVehicle(vehicle))
    {
        //check if points changed
        
        bool changed = CheckIfPointsChanged(vehicle);

        if(!changed) return;

        Log::Level(LOG_LEVEL::LOG_BOTH) << "RotateObjectManager: Changed. Delete data" << std::endl;

        ClearDataForVehicle(vehicle);
    }

    auto modelId = vehicle->modelId;

    if (!ModelInfos::HasModelInfo(modelId)) return;

    AddVehicle(vehicle);
}

void RotateObjectManager::AddVehicle(Vehicle* vehicle)
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "RotateObjectManager: Add vehicle" << std::endl;

    auto modelId = vehicle->modelId;
    auto modelInfo = ModelInfos::GetModelInfo(modelId);

    RotateData* rotateData = new RotateData();

    for (auto lightGroup : modelInfo->lightGroups)
    {
        for (int i = 0; i < (int)lightGroup->points.size(); i++)
        {
            auto point = lightGroup->points[i];

            PointRotateData* pointData = new PointRotateData();

            rotateData->pointsData[point] = pointData;
        }
    }

    m_RotateData[vehicle] = rotateData;
}

PointRotateData* RotateObjectManager::GetPointRotateData(Vehicle* vehicle, Point* point)
{
    if(!HasVehicle(vehicle))
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "Vehicle has not been added" << std::endl;

        ProcessVehicle(vehicle);

        if(!HasVehicle(vehicle))
        {
            Log::Level(LOG_LEVEL::LOG_BOTH) << "what" << std::endl;
        }
    }

    auto rotateData = m_RotateData[vehicle];

    if(rotateData->pointsData.find(point) == rotateData->pointsData.end())
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "Point has not been added" << std::endl;
        return NULL;
    }

    return rotateData->pointsData[point];
}

bool RotateObjectManager::HasVehicle(Vehicle* vehicle)
{
    return m_RotateData.find(vehicle) != m_RotateData.end();
}

bool RotateObjectManager::CheckIfPointsChanged(Vehicle* vehicle)
{
    std::vector<Point*> oldPoints;
    std::vector<Point*> newPoints;

    auto modelId = vehicle->modelId;
    auto modelInfo = ModelInfos::GetModelInfo(modelId);

    //

    for (auto lightGroup : modelInfo->lightGroups)
    {
        for (int i = 0; i < (int)lightGroup->points.size(); i++)
        {
            auto point = lightGroup->points[i];
            newPoints.push_back(point);
        }
    }

    //

    auto rotateData = m_RotateData[vehicle];
    for (const auto& pair : rotateData->pointsData)
    {
        auto point = pair.first;
        oldPoints.push_back(point);
    }

    //

    //fuck it, we compare sizes
    return oldPoints.size() != newPoints.size();
}

void RotateObjectManager::ClearDataForVehicle(Vehicle* vehicle)
{
    auto rotateData = m_RotateData[vehicle];

    for (const auto& pair : rotateData->pointsData)
    {
        auto point = pair.first;
        PointRotateData* data = pair.second;

        delete data;
    }

    rotateData->pointsData.clear();

    m_RotateData.erase(vehicle);
}

void RotateObjectManager::ClearDataForAllVehicles()
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "RotateObjectManager: Clearing data for all vehicles" << std::endl;

    std::vector<Vehicle*> vehiclesToDelete;

    for (auto p : m_RotateData)
    {
        auto vehicle = p.first;
        vehiclesToDelete.push_back(vehicle);
    }

    for (auto vehicle : vehiclesToDelete)
    {
        ClearDataForVehicle(vehicle);
    }
}