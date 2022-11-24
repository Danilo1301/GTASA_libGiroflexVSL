#include "INIConfig.h"

#include "ModelInfos.h"
#include "Log.h"

#include "mod/amlmod.h"
#include "mod/logger.h"
#include "mod/config.h"

bool isDirExist(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

void INIConfig::MakePaths()
{
    Log::file << "INIConfig: MakePaths" << std::endl;

    CreateFolder(GetConfigFolder());
    CreateFolder(GetConfigFolder() + "/vehicles");
}

std::string INIConfig::GetConfigFolder()
{
    char path[0xFF];
    snprintf(path, sizeof(path), "%s/giroflex", aml->GetConfigPath());
    return path;
}

void INIConfig::CreateFolder(std::string path)
{
    if (isDirExist(path)) return;

    Log::file << "INIConfig: CreateFolder " << path << std::endl;

    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void INIConfig::Save()
{
    Log::file << "INIConfig: Save " << std::endl;

    //MakePaths();

    for (auto pair : ModelInfos::m_ModelInfos)
    {
        auto modelInfo = pair.second;


        auto path = GetConfigFolder() + "/vehicles/" + std::to_string(modelInfo->modelId) + ".ini";
        INIFile file(path);
        file.Open();

        file.AddLine(";" + std::to_string(modelInfo->modelId));

        Log::file << "INIConfig: Saving: " << path << std::endl;

        for (auto lightGroup : modelInfo->lightGroups)
        {
            file.AddLine("[LightGroup]");

            file.AddCVector("offset", lightGroup->offset);
            file.AddCRGBA("color1", lightGroup->color1);
            file.AddCRGBA("color2", lightGroup->color2);
            file.AddCRGBA("color3", lightGroup->color3);
            file.AddFloat("radius", lightGroup->radius);

            file.AddBool("renderShadow", lightGroup->renderShadow);
            file.AddFloat("shadowIntensity", lightGroup->shadowIntensity);
            file.AddFloat("shadowSize", lightGroup->shadowSize);

            file.AddBool("renderPointLight", lightGroup->renderPointLight);
            file.AddFloat("pointLightIntensity", lightGroup->pointLightIntensity);
            file.AddFloat("pointLightDistance", lightGroup->pointLightDistance);

            file.AddFloat("nearClip", lightGroup->nearClip);
            file.AddInt("patternOffset", lightGroup->patternOffset);
            file.AddFloat("distance", lightGroup->distance);

            file.AddInt("type", (int)lightGroup->type);
            file.AddBool("usePointPositionInsteadOfIndex", lightGroup->usePointPositionInsteadOfIndex);
            file.AddBool("useSmallWhiteCorona", lightGroup->useSmallWhiteCorona);

            file.AddLine("");
        }

        file.Close();
    }
}

void INIConfig::Load()
{
    MakePaths();

    Log::file << "INIConfig: Load" << std::endl;

    auto path = GetConfigFolder() + "/vehicles/";

    DIR* dirp;
    struct dirent* dp;

    if ((dirp = opendir(path.c_str())) == NULL) {
        //perror("couldn't open '.'");
        return;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        std::string name = dp->d_name;

        //Log::file << name << std::endl;

        int modelId;
        if (sscanf(name.c_str(), "%d.ini", &modelId) == 1)
        {
            Log::file << "INIConfig: Load model id " << modelId << std::endl;

            INIFile file(path + name);
            file.Read();

            if (!ModelInfos::HasModelInfo(modelId)) ModelInfos::CreateModelInfo(modelId);
            auto modelInfo = ModelInfos::GetModelInfo(modelId);

            Log::file << "Found " << file.GetSections("LightGroup").size() << " lightgroups" << std::endl;

            auto sections = file.GetSections("LightGroup");
            for (auto section : sections)
            {
                LightGroup* lightGroup = new LightGroup();

                lightGroup->offset = section.GetCVector("offset");
                lightGroup->color1 = section.GetCRGBA("color1");
                lightGroup->color2 = section.GetCRGBA("color2");
                lightGroup->color3 = section.GetCRGBA("color3");

                lightGroup->radius = section.GetFloat("radius", lightGroup->radius);

                lightGroup->renderShadow = section.GetBool("renderShadow", lightGroup->renderShadow);
                lightGroup->shadowIntensity = section.GetFloat("shadowIntensity", lightGroup->shadowIntensity);
                lightGroup->shadowSize = section.GetFloat("shadowSize", lightGroup->shadowSize);

                lightGroup->renderPointLight = section.GetBool("renderPointLight", lightGroup->renderPointLight);
                lightGroup->pointLightIntensity = section.GetFloat("pointLightIntensity", lightGroup->pointLightIntensity);
                lightGroup->pointLightDistance = section.GetFloat("pointLightDistance", lightGroup->pointLightDistance);

                lightGroup->nearClip = section.GetFloat("nearClip", lightGroup->nearClip);

                lightGroup->patternOffset = section.GetInt("patternOffset", lightGroup->patternOffset);
                lightGroup->distance = section.GetFloat("distance", lightGroup->distance);
                lightGroup->type = (eLightGroupType)section.GetInt("type", (int)lightGroup->type);
                lightGroup->usePointPositionInsteadOfIndex = section.GetBool("usePointPositionInsteadOfIndex", lightGroup->usePointPositionInsteadOfIndex);
                lightGroup->useSmallWhiteCorona = section.GetBool("useSmallWhiteCorona", lightGroup->useSmallWhiteCorona);

                lightGroup->MakeLightGroup();

                modelInfo->lightGroups.push_back(lightGroup);
            }

           

            

        }
    }
}