#include "ConvertOldVersion.h"

#include <dirent.h>
#include "dlfcn.h"

#include "Log.h"
#include "ModConfig.h"
#include "Patterns.h"
#include "eLightGroupType_v2.h"
#include "ModelInfos.h"

#include "iniconfig/INIFile.h"

void ConvertOldVersion::MakePaths()
{
    //ModConfig::CreateFolder(ModConfig::GetConfigFolder() + "/convertFiles");
    //ModConfig::CreateFolder(ModConfig::GetConfigFolder() + "/convertFiles/vehicles");
    //ModConfig::CreateFolder(ModConfig::GetConfigFolder() + "/convertFiles/patterns");
}

void ConvertOldVersion::Load()
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "ConvertOldVersion: Load" << std::endl;

    LoadPatterns();
    LoadVehicles();
}

void ConvertOldVersion::LoadPatterns()
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "ConvertOldVersion: LoadPatterns" << std::endl;

    auto patternsPath = ModConfig::GetConfigFolder() + "/patterns/";

    DIR* dirp;
    struct dirent* dp;

    if ((dirp = opendir(patternsPath.c_str())) == NULL) {
        //perror("couldn't open '.'");
        return;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        std::string name = dp->d_name;

        if (name.find(".ini") == std::string::npos) continue;
        
        std::string id = name.substr(0, name.find("."));

        std::string path = patternsPath + name;

        Log::Level(LOG_LEVEL::LOG_BOTH) << "ConvertOldVersion: Loading pattern '" << name << "' (" << id << ") (" << name << ")" << std::endl;

        //

        Pattern* pattern = NULL;
        if(Patterns::HasPattern(id))
        {
            Log::Level(LOG_LEVEL::LOG_BOTH) << "Pattern already exists. Removing steps..." << std::endl;

            pattern = Patterns::GetPatternById(id);
            pattern->RemoveAllSteps();
        } else {
            pattern = Patterns::CreatePattern(id);
        }

        //

        INIFile file;
        file.Read(path);

        auto sections = file.GetSections("Pattern");
        if (sections.size() == 0) continue;
        auto section = sections[0];

        //
     
        for (auto value : section->values)
        {
            auto line = value.second;

            //Log::Level(LOG_LEVEL::LOG_BOTH) << line << std::endl;

            auto patternStr = line.substr(0, line.find("|"));
            auto timeStr = line.substr(line.find("|") + 1);

            auto time = std::atoi(timeStr.c_str());

            //Log::Level(LOG_LEVEL::LOG_BOTH) << "pattern=" << patternStr << std::endl;
            //Log::Level(LOG_LEVEL::LOG_BOTH) << "time=" << time << std::endl;

            std::vector<int> data;
            for (char& c : patternStr)
            {
                auto value = c == '1' ? 1 : 0;
                data.push_back(value);

                //Log::Level(LOG_LEVEL::LOG_BOTH) << "value: " << value << std::endl;
            }

            pattern->AddStep(data, time);
        }

        //

        file.Destroy();

        ModConfig::ConfigDeleteFile(path);
    }
}

void ConvertOldVersion::LoadVehicles()
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "ConvertOldVersion: LoadVehicles" << std::endl;

    auto vehiclesPath = ModConfig::GetConfigFolder() + "/vehicles/";

    DIR* dirp;
    struct dirent* dp;

    if ((dirp = opendir(vehiclesPath.c_str())) == NULL) {
        //perror("couldn't open '.'");
        return;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        std::string name = dp->d_name;

        //Log::Level(LOG_LEVEL::LOG_BOTH) << name << std::endl;

        if (name.find(".ini") == std::string::npos) continue;

        int modelId;
        if (sscanf(name.c_str(), "%d.ini", &modelId) == 1)
        {
            Log::Level(LOG_LEVEL::LOG_BOTH) << "ConvertOldVersion: Load model ID " << modelId << " (" << name << ")" << std::endl;

            std::string path = vehiclesPath + name;

            INIFile file;
            file.Read(path);

            ModelInfo* modelInfo = NULL;
            if(ModelInfos::HasModelInfo(modelId))
            {
                Log::Level(LOG_LEVEL::LOG_BOTH) << "ModelInfo already exists. Removing lightGroups..." << std::endl;

                modelInfo = ModelInfos::GetModelInfo(modelId);
                modelInfo->RemoveAllLightGroups();
            } else {
                modelInfo = ModelInfos::CreateModelInfo(modelId);
            }

            Log::Level(LOG_LEVEL::LOG_BOTH) << "Loading " << file.GetSections("LightGroup").size() << " lightgroups" << std::endl;

            auto sections = file.GetSections("LightGroup");
            for (auto section : sections)
            {
                LightGroup* lightGroup = new LightGroup();

                LightGroupFromINISection(lightGroup, section);

                modelInfo->lightGroups.push_back(lightGroup);
            }

            file.Destroy();

            ModConfig::ConfigDeleteFile(path);
        }
    }
}

void ConvertOldVersion::LightGroupFromINISection(LightGroup* lightGroup, INISection* section)
{
    lightGroup->offset = section->GetCVector("offset", lightGroup->offset);

    eLightGroupType_v2 type = (eLightGroupType_v2)section->GetInt("type", (int)eLightGroupType_v2::TWO_LIGHTS);
    int amountOfLights = 1;
    switch (type)
    {
    case eLightGroupType_v2::SINGLE_LIGHT:
        break;
    case eLightGroupType_v2::TWO_LIGHTS:
        amountOfLights = 2;
        break;
    case eLightGroupType_v2::THREE_LIGHTS:
        amountOfLights = 3;
        break;
    case eLightGroupType_v2::FOUR_LIGHTS:
        amountOfLights = 4;
        break;
    case eLightGroupType_v2::FIVE_LIGHTS:
        amountOfLights = 5;
        break;
    case eLightGroupType_v2::SIX_LIGHTS:
        amountOfLights = 6;
        break;
    case eLightGroupType_v2::SEVEN_LIGHTS:
        amountOfLights = 7;
        break;
    case eLightGroupType_v2::EIGHT_LIGHTS:
        amountOfLights = 8;
        break;
    case eLightGroupType_v2::TEN_LIGHTS:
        amountOfLights = 10;
        break;
    default:
        break;
    }
    lightGroup->SetNumberOfPoints(amountOfLights);

    lightGroup->distance = section->GetFloat("distance", lightGroup->distance);
    lightGroup->curve = section->GetFloat("curve", lightGroup->curve);

    lightGroup->color1 = section->GetCRGBA("color1", lightGroup->color1);
    lightGroup->color2 = section->GetCRGBA("color2", lightGroup->color2);
    lightGroup->color3 = section->GetCRGBA("color3", lightGroup->color3);

    lightGroup->patternOffset = section->GetInt("patternOffset", lightGroup->patternOffset);

    lightGroup->radius = section->GetFloat("radius", lightGroup->radius);

    lightGroup->renderShadow = section->GetBool("renderShadow", lightGroup->renderShadow);
    lightGroup->shadowIntensity = section->GetFloat("shadowIntensity", lightGroup->shadowIntensity);
    lightGroup->shadowSize = section->GetFloat("shadowSize", lightGroup->shadowSize);
    lightGroup->shadowPositionX = section->GetFloat("shadowPositionX", lightGroup->shadowPositionX);
    lightGroup->shadowPositionY = section->GetFloat("shadowPositionY", lightGroup->shadowPositionY);
    lightGroup->shadowTexture = section->GetInt("shadowTexture", lightGroup->shadowTexture);
    lightGroup->shadowRotation = section->GetFloat("shadowRotation", lightGroup->shadowRotation);
    lightGroup->shadowFlipTextures = section->GetBool("shadowFlipTextures", lightGroup->shadowFlipTextures);

    lightGroup->renderPointLight = section->GetBool("renderPointLight", lightGroup->renderPointLight);
    lightGroup->pointLightIntensity = section->GetFloat("pointLightIntensity", lightGroup->pointLightIntensity);
    lightGroup->pointLightDistance = section->GetFloat("pointLightDistance", lightGroup->pointLightDistance);

    lightGroup->nearClip = section->GetFloat("nearClip", lightGroup->nearClip);

    //lightGroup->usePointPositionInsteadOfIndex = section->GetBool("usePointPositionInsteadOfIndex", lightGroup->usePointPositionInsteadOfIndex);
    
    lightGroup->useSmallWhiteCorona = section->GetBool("useSmallWhiteCorona", lightGroup->useSmallWhiteCorona);
    lightGroup->smallWhiteCoronaScale = section->GetFloat("smallWhiteCoronaScale", lightGroup->smallWhiteCoronaScale);
    lightGroup->smallWhiteCoronaTexture = section->GetInt("smallWhiteCoronaTexture", lightGroup->smallWhiteCoronaTexture);
    lightGroup->smallWhiteCoronaColor = section->GetCRGBA("smallWhiteCoronaColor", lightGroup->smallWhiteCoronaColor);
    
    lightGroup->freezeLights = section->GetBool("freezeLights", lightGroup->freezeLights);

    lightGroup->useFlare = section->GetBool("useFlare", lightGroup->useFlare);
    lightGroup->flareDistance = section->GetFloat("flareDistance", lightGroup->flareDistance);
    lightGroup->flareIntensity = section->GetFloat("flareIntensity", lightGroup->flareIntensity);
    lightGroup->flareTexture = section->GetInt("flareTexture", lightGroup->flareTexture);

    lightGroup->coronaTexture = section->GetInt("coronaTexture", lightGroup->coronaTexture);

    lightGroup->lightSlotId = section->GetInt("lightSlotId", lightGroup->lightSlotId);

    //lightGroup->enableOnSpawn = section->GetBool("enableOnSpawn", lightGroup->enableOnSpawn);
}