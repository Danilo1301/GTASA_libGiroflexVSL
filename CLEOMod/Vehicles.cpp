#include "Vehicles.h"

#include "Log.h"
#include "ModelInfos.h"
#include "Patterns.h"
#include "LightGroupDatas.h"

std::map<int, Vehicle*> Vehicles::m_Vehicles;
std::vector<RenderCorona> Vehicles::m_CoronasToRender;

//std::vector<int> _vehicleExist;

void Vehicles::TryCreateVehicle(int hVehicle, int modelId)
{
	//_vehicleExist.push_back(hVehicle);

	if (HasVehicleHandle(hVehicle)) return;

	Log::file << "Vehicles: Add vehicle " << hVehicle << " (id: " << modelId << ") (" << std::to_string(m_Vehicles.size() + 1) << " total). Modelinfo? " << (ModelInfos::HasModelInfo(modelId) ? "yes" : "no") << std::endl;

	m_Vehicles[hVehicle] = new Vehicle(hVehicle, modelId);
}

bool Vehicles::HasVehicleHandle(int hVehicle)
{
	return m_Vehicles.find(hVehicle) != m_Vehicles.end();
}

Vehicle* Vehicles::GetVehicleByHandle(int hVehicle)
{
	if (!HasVehicleHandle(hVehicle)) return NULL;
	return m_Vehicles.at(hVehicle);
}


Vehicle* Vehicles::GetVehicleByVecIndex(int index)
{
    int i = 0;
    for (auto pair : m_Vehicles)
    {
        if (i == index) return pair.second;
        i++;
    }
    return NULL;
}

void Vehicles::RemoveVehicle(int hVehicle) {
    if (!Vehicles::HasVehicleHandle(hVehicle)) return;

	Vehicle* vehicle = m_Vehicles[hVehicle];

	Log::file << "Vehicles: Remove vehicle " << hVehicle << " (id: " << vehicle->modelId << ") (" << std::to_string(m_Vehicles.size() - 1) << " total)" << std::endl;

	m_Vehicles.erase(hVehicle);
	vehicle->Destroy();
	delete vehicle;
}

void Vehicles::CheckStreamedOutVehicles()
{
    std::vector<int> toRemove;

    for (auto pair : m_Vehicles)
    {
        auto vehicle = pair.second;

        if(vehicle->canBeRemoved) toRemove.push_back(vehicle->hVehicle);
    }

    for (auto hVehicle : toRemove)
    {
        RemoveVehicle(hVehicle);
    }

    /*
    std::vector<int> toRemove;

    for (auto pair : Vehicles::m_Vehicles)
    {
        auto hVehicle = pair.first;

        bool stillExists = false;
        for (auto n : _vehicleExist)
        {
            if (n == hVehicle) {
                stillExists = true;
                continue;
            }
        }

        if (!stillExists)
        {
            toRemove.push_back(hVehicle);
        }
    }

    for (auto hVehicle : toRemove)
    {
        RemoveVehicle(hVehicle);
    }

    _vehicleExist.clear();
    */
}

void Vehicles::Update(int dt)
{
    CheckStreamedOutVehicles();

    //Log::file << "Vehicles: " << m_CoronasToRender.size() << " coronas were rendered" << std::endl;
    m_CoronasToRender.clear();

    for (auto pair : Vehicles::m_Vehicles)
    {
        auto vehicle = pair.second;
        vehicle->Update(dt);
    }
}

void Vehicles::AddCoronaToRender(RenderCorona corona)
{
    if (corona.color.a == 0) return;
    if (corona.radius <= 0.0f) return;

    m_CoronasToRender.push_back(corona);
}





