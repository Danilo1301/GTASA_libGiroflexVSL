#include "SoundPanelSystem.h"

#include "ModConfig.h"
#include "Log.h"
#include "Globals.h"

#include "iniconfig/INIFile.h"

std::string SoundPanelSystem::defaultGroupId = "";
std::map<std::string, SoundGroup*> SoundPanelSystem::soundGroups;
SoundGroup* SoundPanelSystem::prevSoundGroup = NULL;
AudioStream* SoundPanelSystem::prevSiren = NULL;
bool SoundPanelSystem::sirenState = false;
bool SoundPanelSystem::hornState = false;

int SoundPanelSystem::currentSirenIndex = 0;

extern IBASS* BASS;

void SoundPanelSystem::Load()
{
	Log::file << "SoundPanelSystem: Load" << std::endl;

	if (!BASS)
	{
		Log::file << "SoundPanelSystem: not loading (BASS was not loaded)" << std::endl;
		return;
	}

	std::string audiosFolder = ModConfig::GetConfigFolder() + "/audios";
	std::string vehiclesFolder = audiosFolder + "/vehicles";

	if (!ModConfig::DirExists(audiosFolder)) return;
	if (!ModConfig::DirExists(vehiclesFolder)) return;

	LoadConfig();
	LoadVehicles();

	auto defaultSoundGroup = GetDefaultSoundGroup();

	Log::file << "SoundPanelSystem: GetDefaultSoundGroup = " << defaultSoundGroup->id << std::endl;

	auto soundGroupFor416 = GetSoundGroupForModelId(416);

	Log::file << "SoundPanelSystem: GetSoundGroupForModelId 416 = " << soundGroupFor416->id << std::endl;
}

void SoundPanelSystem::LoadConfig()
{
	std::string audiosFolder = ModConfig::GetConfigFolder() + "/audios";
	std::string vehiclesFolder = audiosFolder + "/vehicles";

	auto configFileDir = vehiclesFolder + "/config.ini";

	Log::file << "SoundPanelSystem: LoadConfig (config.ini)" << std::endl;

	INIFile file;
	if (!file.Read(configFileDir))
	{
		Log::file << "SoundPanelSystem: Error reading config.ini (Not found)" << std::endl;
		return;
	}

	auto sections = file.GetSections("Config");
	if (sections.size() == 0) return;
	auto section = sections[0];

	defaultGroupId = section->GetString("defaultGroup");
}

void SoundPanelSystem::LoadVehicles()
{
	std::string audiosFolder = ModConfig::GetConfigFolder() + "/audios";
	std::string vehiclesFolder = audiosFolder + "/vehicles";

	Log::file << "SoundPanelSystem: LoadVehicles" << std::endl;

	auto directories = ModConfig::GetDirectoriesName(vehiclesFolder);

	for (auto directory : directories)
	{
		std::string id = directory;
		std::string dir = vehiclesFolder + "/" + id;

		Log::file << std::endl;
		Log::file << "SoundPanelSystem: Loading " << id << std::endl;

		SoundGroup* soundGroup = new SoundGroup(id);
		soundGroups[id] = soundGroup;

		//
	
		//vehicles.ini

		auto vehiclesFileDir = dir + "/vehicles.ini";

		Log::file << "SoundPanelSystem: Loading vehicles.ini" << std::endl;

		INIFile file;
		if (!file.Read(vehiclesFileDir))
		{
			Log::file << "SoundPanelSystem: Error reading vehicles.ini (Not found)" << std::endl;
			return;
		}

		auto sections = file.GetSections("Compatible_Vehicles");
		if (sections.size() == 0) return;
		auto section = sections[0];

		float volume = 0.2f;

		for (auto value : section->values)
		{
			Log::file << "Found value (" << value.first << "|" << value.second << ")" << std::endl;

			int vehicleModelId = std::atoi(value.second.c_str());
			soundGroup->compatibleVehicles.push_back(vehicleModelId);
		}

		//

		Log::file << "SoundPanelSystem: Loading horn.wav" << std::endl;

		soundGroup->horn = new AudioStream(dir + "/horn.wav");
		if (soundGroup->horn->streamInternal)
		{
			soundGroup->horn->SetVolume(volume);
			soundGroup->horn->Loop(true);
			//soundGroup->horn->Play();
		}

		int i = 1;
		bool searching = true;
		while (searching)
		{
			std::string audioFileName = "siren" + std::to_string(i) + ".wav";

			Log::file << "SoundPanelSystem: Trying to load: " << audioFileName << std::endl;

			if (!ModConfig::FileExists(dir + "/" + audioFileName))
			{
				searching = false;

				Log::file << "SoundPanelSystem: Could not find: " << audioFileName << std::endl;

				break;
			}

			auto audioStream = new AudioStream(dir + "/" + audioFileName);
			soundGroup->sirens.push_back(audioStream);

			if (audioStream->streamInternal)
			{
				audioStream->SetVolume(volume);
				audioStream->Loop(true);
				//audioStream->Play();
			}

			i++;
		}
	}
}

SoundGroup* SoundPanelSystem::GetSoundGroupById(std::string id)
{
	return soundGroups[id];
}

SoundGroup* SoundPanelSystem::GetDefaultSoundGroup()
{
	SoundGroup* defaultSoundGroup = NULL;

	for (auto pair : soundGroups)
	{
		defaultSoundGroup = pair.second;
		break;
	}

	auto find = GetSoundGroupById(defaultGroupId);
	if (find) defaultSoundGroup = find;

	return defaultSoundGroup;
}

SoundGroup* SoundPanelSystem::GetSoundGroupForModelId(int modelId)
{
	//Log::file << "SoundPanelSystem: GetSoundGroupForModelId " << modelId << std::endl;

	SoundGroup* retSoundGroup = GetDefaultSoundGroup();

	for (auto pair : soundGroups)
	{
		bool hasCompatible = pair.second->IsVehicleModelCopatible(modelId);
		
		if (hasCompatible)
		{
			retSoundGroup = pair.second;
		}
	}

	return retSoundGroup;
}

int SoundPanelSystem::GetCurrentVehicleModelId()
{
	//Log::file << "SoundPanelSystem: GetCurrentVehicleModelId" << std::endl;

	int modelId = -1;

	if (auto veh = Globals::GetPlayerVehicle())
	{
		modelId = veh->modelId;
	}

	return modelId;
}

SoundGroup* SoundPanelSystem::GetCurrentVehicleSoundGroup()
{
	//Log::file << "SoundPanelSystem: GetCurrentVehicleSoundGroup" << std::endl;

	return GetSoundGroupForModelId(GetCurrentVehicleModelId());
}

void SoundPanelSystem::Update(int dt)
{
	//Log::file << "SoundPanelSystem: Update" << std::endl;

	if (soundGroups.size() > 0)
	{
		if (prevSoundGroup && !Globals::IsPlayerInAnyVehicle())
		{
			currentSirenIndex = 0;

			if (prevSiren)
			{
				prevSiren->Pause();
				prevSiren = NULL;
				sirenState = false;
			}

			if (hornState)
			{
				prevSoundGroup->horn->Stop();
				hornState = false;
			}
		}

		auto soundGroup = GetCurrentVehicleSoundGroup();

		prevSoundGroup = soundGroup;
	}

	//Log::file << "SoundPanelSystem: end Update" << std::endl;
}

void SoundPanelSystem::ToggleHorn(bool enabled)
{
	Log::file << "SoundPanelSystem: ToggleHorn" << std::endl;

	hornState = enabled;

	auto soundGroup = GetCurrentVehicleSoundGroup();

	if (enabled)
	{
		soundGroup->horn->Play();
	}
	else {
		soundGroup->horn->Pause();
	}
}

void SoundPanelSystem::ToggleSiren(bool enabled)
{
	Log::file << "SoundPanelSystem: ToggleSiren " << (enabled ? "TRUE" : "FALSE") << std::endl;

	sirenState = enabled;

	auto soundGroup = GetCurrentVehicleSoundGroup();
	auto siren = soundGroup->sirens[currentSirenIndex];

	if (enabled)
	{
		siren->Play();
		prevSiren = siren;
	}
	else {
		siren->Pause();
		prevSiren = NULL;
	}
}

void SoundPanelSystem::ChangeSirenByOne()
{
	Log::file << "SoundPanelSystem: ChangeSirenByOne" << std::endl;

	auto soundGroup = GetCurrentVehicleSoundGroup();
	int max = soundGroup->sirens.size();

	currentSirenIndex++;
	if (currentSirenIndex > max - 1)
	{
		currentSirenIndex = 0;
	}

	if (prevSiren)
	{
		prevSiren->Pause();
		prevSiren = NULL;
	}

	if (sirenState == true)
	{
		ToggleSiren(true);
	}
}