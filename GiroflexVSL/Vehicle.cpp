#include "Vehicle.h"

#include "Log.h"

#include "RenderCorona.h"
#include "Vehicles.h"
#include "ModelInfos.h"
#include "LightGroupDatas.h"
#include "Patterns.h"
#include "Globals.h"
#include "ModConfig.h"

#include "windows/WindowEditing.h"

extern void* (*GetVehicleFromRef)(int);

Vehicle::Vehicle(int hVehicle, int modelId)
{
	this->hVehicle = hVehicle;
	this->modelId = modelId;
    pVehicle = (CVehicle*)GetVehicleFromRef(hVehicle);

    std::string vehicleIdString = "Vehicle " + std::to_string(hVehicle) + ": ";

    Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Created" << std::endl;

    Update(0);

    /*
    auto lightGroupDataList = GetLightGroupsData();
    for (auto lightGroupData : lightGroupDataList)
    {
        if(lightGroupData->lightGroup->enableOnSpawn)
            lightGroupData->lightsOn = true;
    }
    */
}

Vehicle::~Vehicle()
{
    Destroy();
}

void Vehicle::Destroy()
{
    std::string vehicleIdString = "Vehicle " + std::to_string(hVehicle) + ": ";

    Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Destroy" << std::endl;
}

void Vehicle::Update(int dt)
{
    std::string vehicleIdString = "Vehicle " + std::to_string(hVehicle) + ": ";

    //Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Update" << std::endl;

    if (!ModelInfos::HasModelInfo(modelId)) return;

    auto modelInfo = ModelInfos::GetModelInfo(modelId);

    auto lightId = hVehicle + 69420;

    for (auto lightGroup : modelInfo->lightGroups)
    {
        if (lightGroup->points.size() == 0)
        {
            Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Warning: lightGroup (from id: " << modelInfo->modelId << ") has 0 points" << std::endl;
            continue;
        }

        //

        if (!LightGroupDatas::HasLightGroupData(lightGroup, hVehicle))
        {   
            std::vector<Pattern*> compatiblePatterns;
            for (auto pattern : Patterns::m_Patterns)
            {
                if (pattern->steps[0]->data.size() != lightGroup->points.size()) continue;
                compatiblePatterns.push_back(pattern);
            }

            if(compatiblePatterns.size() == 0)
            {
                Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Warning: lightGroup (from id: " << modelInfo->modelId << ") has 0 compatible patterns" << std::endl;
                continue;
            }

            LightGroupData* lightGroupData = new LightGroupData(lightGroup, hVehicle);

            for (auto pattern : compatiblePatterns)
                lightGroupData->patterns.push_back(pattern);
           
            LightGroupDatas::AddLightGroupData(lightGroupData);

            Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Created LightGroupData with " << lightGroupData->patterns.size() << " patterns [global: " << LightGroupDatas::m_LightGroupDatas.size() << "]" << std::endl;
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

        for (int i = 0; i < (int)lightGroup->points.size(); i++)
        {
            auto point = lightGroup->points[i];

            auto amountOfPoints = lightGroup->points.size();
            auto distance = lightGroup->distance;
            auto curve = lightGroup->curve;

            //position
            float x = (i * distance) - ((amountOfPoints - 1) * distance / 2);
			float y = (float)arch_fn_parabola((float)i, curve, (float)(amountOfPoints - 1));

            //color
            CRGBA color = lightGroup->color1;
            if((double)i < ((double)amountOfPoints)/(double)2)
            {
                color = lightGroup->color1;

                if(amountOfPoints%2 == 1 && i == std::round(amountOfPoints/2) && amountOfPoints > 2)            
                {
                    color = lightGroup->color3;
                }
            } else {
                color = lightGroup->color2;
            }

            //custom color
            if(point->useCustomColor)
            {
                color = point->customColor;
            }

            //
            int index = i;

            bool enabled = lightGroupData->GetPointIsEnabled(point, index);
            
            if (lightGroup->freezeLights) enabled = true;

            if (!lightGroupData->lightsOn && !lightGroup->alwaysEnabled) enabled = false;

            if (WindowEditing::FreezeLights) enabled = true;

            if (WindowEditing::ShowCurrentEditingLightGroup)
            {
                if (WindowEditing::LightGroupToShow != lightGroup) enabled = false;
            }

            RenderCorona corona;
            corona.car = hVehicle;
            corona.pVehicle = pVehicle;
            corona.id = lightId++;
            corona.color = color;
            corona.offset = CVector(x, y, 0) + lightGroup->offset + point->customOffset;
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
        }
    }

    //-----------

    //Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Getting siren state: " << std::endl;

    //void* vehicleEntity = Mod::ModGetVehicleFromRef(hVehicle);
    //bool sirenOn = *(uint8_t*)((uintptr_t)vehicleEntity + 0x42D + 4) >> 7;

    //Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "siren state: " << sirenOn << std::endl;

    //if (prevLightsState != gameSirenState)
    if (prevSirenState != gameSirenState)
    {
        prevSirenState = gameSirenState;

        if (pDriver)
        {
            if(Globals::hPlayerVehicle == hVehicle)
            {
                //player vehicle
                if(ModConfig::TurnOnLightsWithSiren)
                {
                    SetGiroflexEnabled(gameSirenState);
                }
            } else {
                //npc vehicle
                SetGiroflexEnabled(gameSirenState);
            }
        }
    }

    //Log::Level(LOG_LEVEL::LOG_BOTH) << vehicleIdString << "Update end" << std::endl;
}

std::vector<LightGroupData*> Vehicle::GetLightGroupsData()
{
	std::vector<LightGroupData*> lightGroupDataList;

	if (!ModelInfos::HasModelInfo(this->modelId)) return lightGroupDataList;

	auto modelInfo = ModelInfos::GetModelInfo(this->modelId);

	for (auto lightGroup : modelInfo->lightGroups)
	{
        auto lightGroupData = LightGroupDatas::GetLightGroupData(lightGroup, this->hVehicle);

        if(lightGroupData)
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