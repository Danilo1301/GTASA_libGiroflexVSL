#include "Vehicles.h"

#include "Log.h"
#include "ModelInfos.h"
#include "Patterns.h"
#include "LightGroupDatas.h"
#include "Mod.h"

std::map<int, Vehicle*> Vehicles::m_Vehicles;
std::vector<RenderCorona> Vehicles::m_CoronasToRender;
std::vector<int> Vehicles::m_NewVehiclesRef;

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

void Vehicles::TryFindNewVehicles()
{
    //Log::file << "TryFindNewVehicles" << std::endl;

    m_NewVehiclesRef.clear();

    auto objects = *(GTAVehicleSA**)(*Mod::pVehiclePool + 0);
    tByteFlag* flags = *(tByteFlag**)(*Mod::pVehiclePool + 4);
    int size = *(int*)(*Mod::pVehiclePool + 8);

    for (int i = 0; i < size; ++i)
    {
        if (flags[i].bEmpty) continue;
        auto& ent = objects[i];
        
        //bool sirenOn = *(uint8_t*)((uintptr_t)vehicle + 0x42D + 4) >> 7;
        bool sirenOn = ent.UInt8At(0x42D + 4) >> 7;

        void* driver = (void*)ent.UIntAt(0x460 + 4);

        int ref = Mod::ModGetVehicleRef(ent.AsInt());

        //Log::file << "Found " << ent.AsInt() << " ref " << ref << " siren " << (sirenOn ? "ON" : "OFF") << std::endl;
        //Log::file << "driver " << driver << std::endl;

        for (auto vpair : Vehicles::m_Vehicles)
        {
            auto vehicle = vpair.second;

            if (vehicle->hVehicle != ref) continue;
            
            vehicle->gameSirenState = sirenOn;
            vehicle->pDriver = driver;
        }

        m_NewVehiclesRef.push_back(ref); //important

    }

    //Log::file << "Found: " << m_NewVehiclesRef.size() << std::endl;
}
