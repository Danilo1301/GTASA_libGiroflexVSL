#include "Vehicle.h"
#include "LightGroupDatas.h"
#include "ModelInfos.h"
#include "Log.h"
#include "Patterns.h"
#include "Mod.h"

#include "Vehicles.h"

#include "windows/WindowEditing.h"

Vehicle::Vehicle(int hVehicle, int modelId)
{
	this->hVehicle = hVehicle;
	this->modelId = modelId;

    Update(0);

    auto lightGroupDataList = GetLightGroupsData();
    for (auto lightGroupData : lightGroupDataList)
    {
        if(lightGroupData->lightGroup->enableOnSpawn)
            lightGroupData->lightsOn = true;
    }
}

void Vehicle::Destroy()
{
    std::string vehicleIdString = "Vehicle " + std::to_string(hVehicle) + ": ";

    Log::file << vehicleIdString << "Destroy" << std::endl;

    LightGroupDatas::RemoveLightGroupDataFromVehicle(hVehicle);
}

void Vehicle::Update(int dt)
{
    std::string vehicleIdString = "Vehicle " + std::to_string(hVehicle) + ": ";

    //Log::file << vehicleIdString << "Update" << std::endl;

    pVehicle = Mod::ModGetVehicleFromRef(hVehicle);

    if (!ModelInfos::HasModelInfo(modelId)) return;

    auto modelInfo = ModelInfos::GetModelInfo(modelId);

    auto lightId = hVehicle + 69420;

    for (auto lightGroup : modelInfo->lightGroups)
    {
        if (lightGroup->points.size() == 0)
        {
            Log::file << vehicleIdString << "Warning: lightGroup (from id: " << modelInfo->modelId << ") has 0 points" << std::endl;
            continue;
        }

        //

        if (!LightGroupDatas::HasLightGroupData(lightGroup, hVehicle))
        {
            LightGroupData* lightGroupData = new LightGroupData(lightGroup, hVehicle);

            for (auto pattern : Patterns::m_Patterns)
            {
                if (pattern->steps[0]->data.size() != lightGroup->points.size()) continue;

                lightGroupData->patterns.push_back(pattern);
            }
           
            LightGroupDatas::AddLightGroupData(lightGroupData);

            Log::file << vehicleIdString << "Created LightGroupData with " << lightGroupData->patterns.size() << " patterns [global: " << LightGroupDatas::m_LightGroupDatas.size() << "]" << std::endl;
        }

        //if (!lightsOn) continue;

        LightGroupData* lightGroupData = LightGroupDatas::GetLightGroupData(lightGroup, hVehicle);

        if (!lightGroupData)
        {
            continue;
        }

        //

        if (lightGroupData->patternLoop->HasNoSteps())
        {
            for (auto pattern : lightGroupData->patterns)
            {
                std::cout << pattern << std::endl;

                lightGroupData->patternLoop->AddStep(&LightGroupDatas::m_ChangePatternTime);
            }

            int patternOffset = lightGroup->patternOffset;
            while (patternOffset > 0)
            {
                lightGroupData->patternLoop->StepIndex++;
                if (lightGroupData->patternLoop->StepIndex >= lightGroupData->patternLoop->Steps.size()) lightGroupData->patternLoop->StepIndex = 0;
                patternOffset--;
            }

        }

       
        //
        //if (!lightsPaused)
        {
            lightGroupData->patternLoop->Update(dt);

            if (lightGroupData->patternLoop->HasStepChanged())
            {
                //std::cout << "patternLoop step changed" << std::endl;
                lightGroupData->stepLoop->Clear();
            }

            //

            Pattern* pattern = lightGroupData->GetCurrentPattern();

            if (pattern == NULL)
            {
                //std::cout << "Pattern not found" << std::endl;
                continue;
            }

            if (pattern->steps.size() == 0)
            {
                //std::cout << "Pattern has no steps" << std::endl;
                continue;
            }

            if (lightGroupData->stepLoop->HasNoSteps())
            {
                Pattern* pattern = lightGroupData->GetCurrentPattern();

                //std::cout << "add " << pattern->steps.size() << " steps to stepLoop" << std::endl;

                for (auto step : pattern->steps)
                {
                    lightGroupData->stepLoop->AddStep(&step->duration);
                }
            }

            lightGroupData->stepLoop->Update(dt);
        }
        //

        std::string debugPoints = "";

        for (int i = 0; i < (int)lightGroup->points.size(); i++)
        {
            auto point = lightGroup->points[i];

            int index = i;
            if (lightGroup->usePointPositionInsteadOfIndex) index = (int)point->pointPosition;

            bool enabled = lightGroupData->GetPointIsEnabled(point, index);

            if (lightGroup->freezeLights) enabled = true;

            if (!lightGroupData->lightsOn) enabled = false;

            if (WindowEditing::FreezeLights) enabled = true;

            if (WindowEditing::ShowCurrentEditingLightGroup)
            {
                if (WindowEditing::LightGroupToShow != lightGroup) enabled = false;
            }

            RenderCorona corona;
            corona.car = hVehicle;
            corona.pVehicle = pVehicle;
            corona.id = lightId++;
            corona.color = lightGroup->GetPointColor(point, index);
            corona.offset = lightGroup->offset + point->offset;
            corona.radius = enabled ? lightGroup->radius : 0.0f;
            corona.renderShadow = enabled ? lightGroup->renderShadow : false;
            corona.renderPointLight = enabled ? lightGroup->renderPointLight : false;

            corona.shadowIntensity = lightGroup->shadowIntensity;
            corona.shadowSize = lightGroup->shadowSize;
            corona.shadowOffsetX = lightGroup->shadowPositionX;
            corona.shadowOffsetY = lightGroup->shadowPositionY;
            corona.shadowTexture = lightGroup->shadowTexture;
            corona.shadowRotation = lightGroup->shadowRotation;
            corona.shadowFlipTextures = lightGroup->shadowFlipTextures;

            corona.pointLightDistance = lightGroup->pointLightDistance;
            corona.pointLightIntensity = lightGroup->pointLightIntensity;

            corona.nearClip = lightGroup->nearClip;
            corona.useFlare = lightGroup->useFlare;
            corona.flareIntensity = lightGroup->flareIntensity;
            corona.flareDistance = lightGroup->flareDistance;
            corona.flareTexture = lightGroup->flareTexture;
            corona.coronaTexture = lightGroup->coronaTexture;
            Vehicles::AddCoronaToRender(corona);

            if(corona.useFlare) lightId++;

            if (lightGroup->useSmallWhiteCorona)
            {
                RenderCorona corona2;
                corona2.car = hVehicle;
                corona2.pVehicle = pVehicle;
                corona2.id = lightId++;
                corona2.color = lightGroup->smallWhiteCoronaColor;
                corona2.offset = corona.offset;
                corona2.radius = corona.radius * lightGroup->smallWhiteCoronaScale;
                corona2.nearClip = corona.nearClip;
                corona2.coronaTexture = lightGroup->smallWhiteCoronaTexture;
                Vehicles::AddCoronaToRender(corona2);
            }

            //

            //get point position at pattern, return state

            //debugPoints += (enabled ? "[#]" : "[ ]");
        }

        //std::cout << "Points: " << debugPoints << " | " << lightGroupData->patternLoop->StepIndex << "/" << lightGroupData->patternLoop->Steps.size() << ", " << lightGroupData->stepLoop->StepIndex << std::endl;
        //Log::file << "Points: " << debugPoints << std::endl;
    }

    //-----------

    //Log::file << vehicleIdString << "Getting siren state: " << std::endl;

    //void* vehicleEntity = Mod::ModGetVehicleFromRef(hVehicle);
    //bool sirenOn = *(uint8_t*)((uintptr_t)vehicleEntity + 0x42D + 4) >> 7;

    //Log::file << vehicleIdString << "siren state: " << sirenOn << std::endl;


    if (Mod::hPlayerVehicle != hVehicle && pDriver)
    {
        if (prevLightsState != gameSirenState)
        {
            SetGiroflexEnabled(gameSirenState);
        }
    }

    //Log::file << vehicleIdString << "Update end" << std::endl;

}


std::vector<LightGroupData*> Vehicle::GetLightGroupsData()
{
	std::vector<LightGroupData*> lightGroupDataList;

	if (!ModelInfos::HasModelInfo(this->modelId)) return lightGroupDataList;

	auto modelInfo = ModelInfos::GetModelInfo(this->modelId);

	for (auto lightGroup : modelInfo->lightGroups)
	{
        auto lightGroupData = LightGroupDatas::GetLightGroupData(lightGroup, this->hVehicle);

        lightGroupDataList.push_back(lightGroupData);
	}

	return lightGroupDataList;
}

void Vehicle::SetGiroflexEnabled(bool enabled)
{
    prevLightsState = enabled;
	//lightsPaused = !enabled;
	//lightsOn = enabled;

	auto lightGroupDataList = GetLightGroupsData();
    for (auto lightGroupData : lightGroupDataList)
    {
        lightGroupData->lightsOn = enabled;
    }

	/*

	std::cout << "lightGroupDataList " << lightGroupDataList.size() << std::endl;

	for (auto lightGroupData : lightGroupDataList)
	{
		lightGroupData->stepLoop->DontChangeSteps = !enabled;
	}
	*/

}