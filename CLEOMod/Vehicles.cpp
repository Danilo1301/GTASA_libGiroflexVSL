#include "Vehicles.h"

#include "Log.h"
#include "ModelInfos.h"
#include "Patterns.h"
#include "LightGroupDatas.h"

std::map<int, Vehicle*> Vehicles::m_Vehicles;

std::vector<int> _vehicleExist;

void Vehicles::TryCreateVehicle(int hVehicle, int modelId)
{
	_vehicleExist.push_back(hVehicle);

	if (HasVehicleHandle(hVehicle)) return;

	Log::file << "Add vehicle " << hVehicle << " (id: " << modelId << ") (" << std::to_string(m_Vehicles.size() + 1) << " total)" << std::endl;
    Log::file << "Has ModelInfo? " << ModelInfos::HasModelInfo(modelId) << std::endl;

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

void Vehicles::RemoveVehicle(int hVehicle) {
    if (!Vehicles::HasVehicleHandle(hVehicle)) return;

	Vehicle* vehicle = m_Vehicles[hVehicle];

	Log::file << "Remove vehicle " << hVehicle << " (id: " << vehicle->modelId << ") (" << std::to_string(m_Vehicles.size() - 1) << " total)" << std::endl;

	m_Vehicles.erase(hVehicle);
	vehicle->Destroy();
	delete vehicle;
}

void Vehicles::CheckStreamedOutVehicles()
{
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
			RemoveVehicle(hVehicle);
		}
	}

	_vehicleExist.clear();
}

void Vehicles::UpdateVehiclesPattern(int dt)
{
    for (auto pair : Vehicles::m_Vehicles)
    {
        auto vehicle = pair.second;

        if (!ModelInfos::HasModelInfo(vehicle->modelId)) continue;

        auto modelInfo = ModelInfos::GetModelInfo(vehicle->modelId);

        for (auto lightGroup : modelInfo->lightGroups)
        {
            if (lightGroup->points.size() == 0)
            {
                std::cout << "Warning: lightGroup (from id: " << modelInfo->modelId << ") has 0 points" << std::endl;
                Log::file << "Warning: lightGroup (from id: " << modelInfo->modelId << ") has 0 points" << std::endl;
                continue;
            }

            if (!LightGroupDatas::HasLightGroupData(lightGroup))
            {
                LightGroupData* lightGroupData = new LightGroupData();

                for (auto pattern : Patterns::m_Patterns)
                {
                    if (pattern->steps[0]->data.size() != lightGroup->points.size()) continue;

                    lightGroupData->patterns.push_back(pattern);
                }

                LightGroupDatas::m_LightGroupDatas[lightGroup] = lightGroupData;

                std::cout << "Created LightGroupData with " << lightGroupData->patterns.size() << " patterns" << std::endl;
                Log::file << "Created LightGroupData with " << lightGroupData->patterns.size() << " patterns" << std::endl;
            }

            LightGroupData* lightGroupData = LightGroupDatas::m_LightGroupDatas[lightGroup];

            if (lightGroupData->patternLoop->HasNoSteps())
            {
                for (auto pattern : lightGroupData->patterns)
                {
                    std::cout << pattern << std::endl;

                    lightGroupData->patternLoop->AddStep(&LightGroupDatas::m_ChangePatternTime);
                }
            }

            lightGroupData->patternLoop->Update(dt);

            if (lightGroupData->patternLoop->HasStepChanged())
            {
                std::cout << "patternLoop step changed" << std::endl;

                lightGroupData->stepLoop->Clear();
            }

            Pattern* pattern = lightGroupData->GetCurrentPattern();

            if (pattern == NULL)
            {
                std::cout << "Pattern not found" << std::endl;
                continue;
            }

            if (pattern->steps.size() == 0)
            {
                std::cout << "Pattern has no steps" << std::endl;
                continue;
            }

            if (lightGroupData->stepLoop->HasNoSteps())
            {
                Pattern* pattern = lightGroupData->GetCurrentPattern();

                std::cout << "add " << pattern->steps.size() << " steps to stepLoop" << std::endl;

                for (auto step : pattern->steps)
                {
                    lightGroupData->stepLoop->AddStep(&step->duration);
                }
            }

            lightGroupData->stepLoop->Update(dt);

            std::string debugPoints = "";

            for (int i = 0; i < (int)lightGroup->points.size(); i++)
            {
                auto point = lightGroup->points[i];

                int index = i;
                if (lightGroup->usePointPositionInsteadOfIndex) index = (int)point->pointPosition;

                bool enabled = lightGroupData->GetPointIsEnabled(point, index);

                //get point position at pattern, return state

                debugPoints += (enabled ? "[#]" : "[ ]");
            }

            //std::cout << "Points: " << debugPoints << " | " << lightGroupData->patternLoop->StepIndex << "/" << lightGroupData->patternLoop->Steps.size() << ", " << lightGroupData->stepLoop->StepIndex << std::endl;
            //Log::file << "Points: " << debugPoints << std::endl;
        }
    }
}