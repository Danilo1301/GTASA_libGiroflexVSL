#include "ModConfig.h"

#include <iostream>
#include <fstream>
    
#include "mod/amlmod.h"
#include "mod/logger.h"
#include "mod/config.h"

#include "Log.h"
#include "GiroflexVSL.h"
#include "ModelInfos.h"
#include "Patterns.h"
#include "SoundPanelSystem.h"
#include "ConvertOldVersion.h"

#include "windows/WindowSoundPanel.h"

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

bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

std::vector<std::string> get_directories_name(const std::string& s)
{
    std::vector<std::string> r;
    for (auto& p : std::filesystem::recursive_directory_iterator(s))
        if (p.is_directory())
            r.push_back(p.path().filename().string());
    return r;
}

//

bool ModConfig::IgnoreOldModVersionMessage = false;

void ModConfig::MakePaths()
{
    Log::file << "ModConfig: MakePaths" << std::endl;

    CreateFolder(GetConfigFolder());
    CreateFolder(GetConfigFolder() + "/vehicles");
    CreateFolder(GetConfigFolder() + "/patterns");
    CreateFolder(GetConfigFolder() + "/audios");

    ConvertOldVersion::MakePaths();
}

bool ModConfig::DirExists(std::string path)
{
    return isDirExist(path);
}

bool ModConfig::FileExists(std::string path)
{
    return file_exists(path);
}

std::vector<std::string> ModConfig::GetDirectoriesName(std::string path)
{
    return get_directories_name(path);
}

void ModConfig::ConfigDeleteFile(std::string path)
{
    try {
        if (std::filesystem::remove(path))
        Log::file << "ModConfig: file " << path << " deleted" << std::endl;
        else
        Log::file << "ModConfig: file " << path << " not found" << std::endl;
    }
    catch(const std::filesystem::filesystem_error& err) {
        Log::file << "ModConfig: delete file: filesystem error: " << err.what() << std::endl;
    }
}

std::string ModConfig::GetConfigFolder()
{
    char path[0xFF];
    snprintf(path, sizeof(path), "%s/giroflexVSL", aml->GetConfigPath());
    return path;
}

void ModConfig::CreateFolder(std::string path)
{
    if (DirExists(path)) return;

    Log::file << "ModConfig: CreateFolder " << path << std::endl;

    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void ModConfig::WriteToFile(std::string path, Json::Value value) {
	Log::file << "ModConfig: WriteToFile " << path << std::endl;

	Json::StyledWriter writer;
	std::string strJson = writer.write(value);

	std::ofstream file(path);
	file << strJson;
	file.close();
}

Json::Value ModConfig::ReadFile(std::string path) {
	Log::file << "ModConfig: ReadFile " << path << std::endl;

	std::ifstream file(path);

	Json::Value value;
	Json::Reader reader;

	if (!reader.parse(file, value, true)) {
		//MessageBox(HWND_DESKTOP, std::string("Error loading " + path).c_str(), "GiroflexVSL", MB_ICONERROR);
	}

	return value;
}

void ModConfig::Save()
{
    Log::file << "ModConfig: Save" << std::endl;

    MakePaths();

    SavePatterns();
    SaveVehicles();
    SaveSettings();
}

void ModConfig::SavePatterns()
{
    Log::file << "ModConfig: SavePatterns " << std::endl;

    for (auto pattern : Patterns::m_Patterns)
    {
        auto path = GetConfigFolder() + "/patterns/" + pattern->id + ".json";

        Log::file << "ModConfig: Saving pattern ID " << pattern->id + ".json" << std::endl;
        
        Json::Value value = pattern->ToJSON();
        
        WriteToFile(path, value);
    }
}

void ModConfig::SaveVehicles()
{
    Log::file << "ModConfig: SaveVehicles " << std::endl;

    for (auto pair : ModelInfos::m_ModelInfos)
    {
        auto modelInfo = pair.second;

        auto path = GetConfigFolder() + "/vehicles/" + std::to_string(modelInfo->modelId) + ".json";

        Log::file << "ModConfig: Saving model ID " << modelInfo->modelId << std::endl;
        Log::file << "Saving " << modelInfo->lightGroups.size() << " lightgroups" << std::endl;

        Json::Value value = Json::objectValue;

        value["lightGroups"] = Json::arrayValue;

        for (auto lightGroup : modelInfo->lightGroups)
        {
            value["lightGroups"].append( lightGroup->ToJSON() );
        }

        value["version"] = GiroflexVSL::m_Version;

	    WriteToFile(path, value);
    }
}

void ModConfig::SaveSettings()
{
    Log::file << "ModConfig: SaveSettings  (settings.ini)" << std::endl;

    auto settingsFileDir = GetConfigFolder() + "/settings.ini";

    INIFile file;

    auto soundPanelSection = file.AddSection("SoundPanel");
    soundPanelSection->AddBool("allow_multiple_sound", WindowSoundPanel::m_allowMultipleSounds);
    soundPanelSection->AddBool("show_on_enter_vehicle", WindowSoundPanel::m_showOnEnterVehicle);
    soundPanelSection->AddBool("show_button_toggle_lights", WindowSoundPanel::m_showButtonToggleLights);
    soundPanelSection->AddCRGBA("button_color", WindowSoundPanel::m_buttonDefaultColor);
    soundPanelSection->AddCRGBA("button_active_color", WindowSoundPanel::m_buttonActiveColor);
    soundPanelSection->AddCRGBA("button_outline_color", WindowSoundPanel::m_buttonOutlineColor);
    soundPanelSection->AddCVector2D("position", WindowSoundPanel::m_position);
    soundPanelSection->AddFloat("button_size", WindowSoundPanel::m_buttonSize);
    soundPanelSection->AddInt("style", WindowSoundPanel::m_style);
    
    auto generalSection = file.AddSection("General");
    generalSection->AddBool("ignore_message_old_version", ModConfig::IgnoreOldModVersionMessage);

    file.Save(settingsFileDir);
    file.Destroy();
}

void ModConfig::Load()
{
    MakePaths();

    LoadPatterns();
    LoadVehicles();
    LoadSettings();

    SoundPanelSystem::Load();

    ConvertOldVersion::Load();
}

void ModConfig::LoadPatterns()
{
    Log::file << "ModConfig: Load patterns..." << std::endl;

    auto patternsPath = GetConfigFolder() + "/patterns/";

    DIR* dirp;
    struct dirent* dp;

    if ((dirp = opendir(patternsPath.c_str())) == NULL) {
        //perror("couldn't open '.'");
        return;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        std::string name = dp->d_name;

        if (name.find(".json") == std::string::npos) continue;
        
        std::string id = name.substr(0, name.find("."));

        std::string path = patternsPath + name;

        Log::file << "Loading pattern '" << name << "' (id: " << id << ") (" << name << ")" << std::endl;

        Json::Value value = ReadFile(path);

        auto pattern = Patterns::CreatePattern(id);

        pattern->FromJSON(value);
    }   
}

void ModConfig::LoadVehicles()
{
    Log::file << "ModConfig: Load vehicles..." << std::endl;

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

        if (name.find(".json") == std::string::npos) continue;

        //Log::file << name << std::endl;

        int modelId;
        if (sscanf(name.c_str(), "%d.json", &modelId) == 1)
        {
            Log::file << "ModConfig: Load model ID " << modelId << " (" << name << ")" << std::endl;

            std::string path = vehiclesPath + name;

            if (!ModelInfos::HasModelInfo(modelId)) ModelInfos::CreateModelInfo(modelId);
            auto modelInfo = ModelInfos::GetModelInfo(modelId);

            Json::Value value = ReadFile(path);

            for (int lightGroup_i = 0; lightGroup_i < (int)value["lightGroups"].size(); lightGroup_i++)
		    {
                LightGroup* lightGroup = new LightGroup();

                lightGroup->FromJSON(value["lightGroups"][lightGroup_i]);

                modelInfo->lightGroups.push_back(lightGroup);
            }
        }
    }
}

void ModConfig::LoadSettings()
{
    auto settingsFileDir = GetConfigFolder() + "/settings.ini";

    Log::file << "ModConfig: LoadSettings  (settings.ini)" << std::endl;

    INIFile file;
    if (!file.Read(settingsFileDir))
    {
        Log::file << "ModConfig: Error reading settings.ini (Not found)" << std::endl;
        return;
    }

    auto soundPanelSections = file.GetSections("SoundPanel");
    if (soundPanelSections.size() > 0)
    {
        auto soundPanelSection = soundPanelSections[0];

        WindowSoundPanel::m_allowMultipleSounds = soundPanelSection->GetBool("allow_multiple_sound", WindowSoundPanel::m_allowMultipleSounds);
        WindowSoundPanel::m_showOnEnterVehicle = soundPanelSection->GetBool("show_on_enter_vehicle", WindowSoundPanel::m_showOnEnterVehicle);
        WindowSoundPanel::m_showButtonToggleLights = soundPanelSection->GetBool("show_button_toggle_lights", WindowSoundPanel::m_showButtonToggleLights);
        WindowSoundPanel::m_buttonDefaultColor = soundPanelSection->GetCRGBA("button_color", WindowSoundPanel::m_buttonDefaultColor);
        WindowSoundPanel::m_buttonActiveColor = soundPanelSection->GetCRGBA("button_active_color", WindowSoundPanel::m_buttonActiveColor);
        WindowSoundPanel::m_buttonOutlineColor = soundPanelSection->GetCRGBA("button_outline_color", WindowSoundPanel::m_buttonOutlineColor);
        WindowSoundPanel::m_position = soundPanelSection->GetCVector2D("position", WindowSoundPanel::m_position);
        WindowSoundPanel::m_buttonSize = soundPanelSection->GetFloat("button_size", WindowSoundPanel::m_buttonSize);
        WindowSoundPanel::m_style = soundPanelSection->GetFloat("style", WindowSoundPanel::m_style);
    }

     auto generalSections = file.GetSections("General");
    if (generalSections.size() > 0)
    {
        auto generalSection = generalSections[0];

        ModConfig::IgnoreOldModVersionMessage = generalSection->GetBool("ignore_message_old_version", ModConfig::IgnoreOldModVersionMessage);
    }

    Log::file << "ModConfig: Success reading settings.ini" << std::endl;
}

std::string ModConfig::ReadVersionFile()
{
    std::string prevVersion = "unknown";
    std::string path = GetConfigFolder() + "/version";

    std::ifstream file;
    file.open(path);
    if (file.good())
    {
        getline(file, prevVersion);
    }
    file.close();

    return prevVersion;
}

void ModConfig::ProcessVersionChanges_PreConfigLoad()
{
    std::string prevVersion = ReadVersionFile();
    std::string currentVersion = GiroflexVSL::m_Version;

    Log::file << "ModConfig: [PRE] Updating from " << prevVersion << " to " << currentVersion << std::endl;

    if (prevVersion == currentVersion) return;

    //-------------

    /*
    if (prevVersion == "2.0.0-example")
    {
        prevVersion = "2.0.1";
    }
    */

    //-------------
}

void ModConfig::ProcessVersionChanges_PostConfigLoad()
{
    std::string prevVersion = ReadVersionFile();
    std::string currentVersion = GiroflexVSL::m_Version;

    Log::file << "ModConfig: [POST] Updating from " << prevVersion << " to " << currentVersion << std::endl;
    
    if (prevVersion == currentVersion) return;

    //-------------

    /*
    if (prevVersion == "2.0.0-example")
    {
        prevVersion = "2.0.1";
    }
    */

    //-------------

    Log::file << "ModConfig: Saving version file" << std::endl;

    std::string path = GetConfigFolder() + "/version";

    std::fstream file;
    file.open(path, std::fstream::out);
    file << currentVersion;
    file.close();
}