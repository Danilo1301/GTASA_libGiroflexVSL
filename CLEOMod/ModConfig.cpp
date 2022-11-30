#include "ModConfig.h"

#include "ModelInfos.h"
#include "Log.h"

#include "mod/amlmod.h"
#include "mod/logger.h"
#include "mod/config.h"

#include "iniconfig/INIFile.h"


bool isDirExist(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

void ModConfig::MakePaths()
{
    Log::file << "ModConfig: MakePaths" << std::endl;

    CreateFolder(GetConfigFolder());
    CreateFolder(GetConfigFolder() + "/vehicles");
}

std::string ModConfig::GetConfigFolder()
{
    char path[0xFF];
    snprintf(path, sizeof(path), "%s/giroflex", aml->GetConfigPath());
    return path;
}

void ModConfig::CreateFolder(std::string path)
{
    if (isDirExist(path)) return;

    Log::file << "ModConfig: CreateFolder " << path << std::endl;

    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void ModConfig::Save()
{
    Log::file << "ModConfig: Save " << std::endl;

    MakePaths();

    for (auto pair : ModelInfos::m_ModelInfos)
    {
        auto modelInfo = pair.second;

        auto path = GetConfigFolder() + "/vehicles/" + std::to_string(modelInfo->modelId) + ".ini";

        INIFile file;

        Log::file << "ModConfig: Saving model ID " << modelInfo->modelId << std::endl;
        Log::file << "Saving " << modelInfo->lightGroups.size() << " lightgroups" << std::endl;

        for (auto lightGroup : modelInfo->lightGroups)
        {
            auto section = lightGroup->ToINISection();
            file.sections.push_back(section);
        }

        file.Save(path);
        file.Destroy();
    }
}

void ModConfig::Load()
{
    MakePaths();

    Log::file << "ModConfig: Load" << std::endl;

    auto vehiclesPath = GetConfigFolder() + "/vehicles/";

    DIR* dirp;
    struct dirent* dp;

    if ((dirp = opendir(vehiclesPath.c_str())) == NULL) {
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
            Log::file << "ModConfig: Load model ID " << modelId << std::endl;

            std::string path = vehiclesPath + name;

            INIFile file;
            file.Read(path);

            if (!ModelInfos::HasModelInfo(modelId)) ModelInfos::CreateModelInfo(modelId);
            auto modelInfo = ModelInfos::GetModelInfo(modelId);

            Log::file << "Loading " << file.GetSections("LightGroup").size() << " lightgroups" << std::endl;

            auto sections = file.GetSections("LightGroup");
            for (auto section : sections)
            {
                LightGroup* lightGroup = new LightGroup();

                lightGroup->FromINISection(section);

                lightGroup->MakeLightGroup();

                modelInfo->lightGroups.push_back(lightGroup);
            }

            file.Destroy();
        }
    }
}