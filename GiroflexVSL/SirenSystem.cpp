#include "SirenSystem.h"

#include "Log.h"
#include "ModConfig.h"
#include "Globals.h"
#include "SoundSystem.h"
#include "Vehicles.h"
#include "Widgets.h"

#include "iniconfig/INIFile.h"

std::string SirenSystem::m_DefaultGroupId = "";
std::map<std::string, SirenGroup*> SirenSystem::m_SirenGroups;
bool SirenSystem::FixLoudSounds = true;

extern IBASS* BASS;
extern CCamera* camera;

void SirenSystem::Load()
{
	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Load" << std::endl;

	if (!BASS)
	{
		Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: not loading (BASS was not loaded)" << std::endl;
		return;
	}

	std::string audiosFolder = ModConfig::GetConfigFolder() + "/audios";
	std::string vehiclesFolder = audiosFolder + "/vehicles";

	if (!ModConfig::DirExists(audiosFolder)) return;
	if (!ModConfig::DirExists(vehiclesFolder)) return;

	LoadConfig();
	LoadVehicles();

	auto defaultSirenGroup = GetDefaultSirenGroup();

	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: GetdefaultSirenGroup = " << defaultSirenGroup->id << std::endl;

	auto soundGroupFor416 = GetSirenGroupForModelId(416);

	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: GetSoundGroupForModelId 416 = " << soundGroupFor416->id << std::endl;
}

void SirenSystem::LoadConfig()
{
	std::string audiosFolder = ModConfig::GetConfigFolder() + "/audios";
	std::string vehiclesFolder = audiosFolder + "/vehicles";

	auto configFileDir = vehiclesFolder + "/config.ini";

	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: LoadConfig (audios/vehicles/config.ini)" << std::endl;

	INIFile file;
	if (!file.Read(configFileDir))
	{
		Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Error reading config.ini (Not found)" << std::endl;
		return;
	}

	auto sections = file.GetSections("Config");
	if (sections.size() == 0) return;
	auto section = sections[0];

	m_DefaultGroupId = section->GetString("defaultGroup");
}

void SirenSystem::LoadVehicles()
{
	std::string audiosFolder = ModConfig::GetConfigFolder() + "/audios";
	std::string vehiclesFolder = audiosFolder + "/vehicles";

	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: LoadVehicles" << std::endl;

	auto directories = ModConfig::GetDirectoriesName(vehiclesFolder);

	for (auto directory : directories)
	{
		std::string id = directory;
		std::string dir = vehiclesFolder + "/" + id;

		Log::Level(LOG_LEVEL::LOG_BOTH) << std::endl;
		Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Loading " << id << std::endl;

		auto sirenGroup = new SirenGroup(id);

		Log::Level(LOG_LEVEL::LOG_BOTH) << "1" << std::endl;

		m_SirenGroups[id] = sirenGroup;

		//
	
		//vehicles.ini

		auto vehiclesFileDir = dir + "/vehicles.ini";

		Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Loading vehicles.ini" << std::endl;

		INIFile file;
		if (!file.Read(vehiclesFileDir))
		{
			Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Error reading vehicles.ini (Not found)" << std::endl;
			return;
		}

		auto sections = file.GetSections("Compatible_Vehicles");
		if (sections.size() == 0) return;
		auto section = sections[0];

		for (auto value : section->values)
		{
			Log::Level(LOG_LEVEL::LOG_BOTH) << "Found value (" << value.first << "|" << value.second << ")" << std::endl;

			int vehicleModelId = std::atoi(value.second.c_str());
			sirenGroup->compatibleVehicles.push_back(vehicleModelId);
		}

		//

		Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Loading horn.wav" << std::endl;

		sirenGroup->hornDir = dir + "/horn.wav";
		
		int i = 1;
		bool searching = true;
		while (searching)
		{
			std::string audioFileName = "siren" + std::to_string(i) + ".wav";

			Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Trying to load: " << audioFileName << std::endl;

			if (!ModConfig::FileExists(dir + "/" + audioFileName))
			{
				searching = false;

				Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Could not find: " << audioFileName << std::endl;

				break;
			}

			sirenGroup->sirensDir.push_back(dir + "/" + audioFileName);

			i++;
		}

        Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Siren group has " << sirenGroup->sirensDir.size() << " sirens" << std::endl;
	}
}

SirenGroup* SirenSystem::GetSirenGroupById(std::string id)
{
	return m_SirenGroups[id];
}

SirenGroup* SirenSystem::GetDefaultSirenGroup()
{
	SirenGroup* defaultSirenGroup = NULL;

	for (auto pair : m_SirenGroups)
	{
		defaultSirenGroup = pair.second;
		break;
	}

	auto find = GetSirenGroupById(m_DefaultGroupId);
	if (find) defaultSirenGroup = find;

	return defaultSirenGroup;
}

SirenGroup* SirenSystem::GetSirenGroupForModelId(int modelId)
{
	//Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: GetSoundGroupForModelId " << modelId << std::endl;

	SirenGroup* retSoundGroup = GetDefaultSirenGroup();

	for (auto pair : m_SirenGroups)
	{
		bool hasCompatible = pair.second->IsVehicleModelCopatible(modelId);
		
		if (hasCompatible)
		{
			retSoundGroup = pair.second;
		}
	}

	return retSoundGroup;
}

bool SirenSystem::ModelIdHasSirenGroup(int modelId)
{
	for(auto pair : m_SirenGroups)
	{
		auto sirenGroup = pair.second;
		
		if(sirenGroup->IsVehicleModelCopatible(modelId)) return true;
	}
	return false;
}

int SirenSystem::GetCurrentVehicleModelId()
{
	//Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: GetCurrentVehicleModelId" << std::endl;

	int modelId = -1;

	if (auto veh = Globals::GetPlayerVehicle())
	{
		modelId = veh->modelId;
	}

	return modelId;
}

SirenGroup* SirenSystem::GetCurrentVehicleSoundGroup()
{
	//Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: GetCurrentVehicleSoundGroup" << std::endl;

	return GetSirenGroupForModelId(GetCurrentVehicleModelId());
}

//--------------------------------------

SirenSystem::SirenSystem(int hVehicle)
{
	this->hVehicle = hVehicle;
}

void SirenSystem::LoadAudios()
{
	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: LoadAudios" << std::endl;

	auto vehicle = Vehicles::GetVehicleByHandle(hVehicle);
	auto modelId = vehicle->modelId;

	auto sirenGroup = SirenSystem::GetSirenGroupForModelId(modelId);

	horn = SoundSystem::LoadStream(sirenGroup->hornDir, true);
	horn->Loop(true);
	horn->Link(vehicle->pVehicle);

	for(auto sirenDir : sirenGroup->sirensDir)
	{
		auto siren = SoundSystem::LoadStream(sirenDir, true);
		siren->Loop(true);
		siren->Link(vehicle->pVehicle);

		sirens.push_back(siren);
	}

	//radio

	std::string audiosFolder = ModConfig::GetConfigFolder() + "/audios";

	radio = SoundSystem::LoadStream(audiosFolder + "/radio/radio1.wav", true);
	radio->Loop(true);
	radio->Link(vehicle->pVehicle);
}

void SirenSystem::Init()
{
	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: Init" << std::endl;

	auto vehicle = Vehicles::GetVehicleByHandle(hVehicle);
	auto modelId = vehicle->modelId;

	if(SirenSystem::ModelIdHasSirenGroup(modelId))
	{
		LoadAudios();
	}
}

void SirenSystem::Update(int dt)
{
	Log::Level(LOG_LEVEL::LOG_DEEP_UPDATE) << "SirenSystem: Update" << std::endl;

	auto vehicle = Vehicles::GetVehicleByHandle(hVehicle);
	auto modelId = vehicle->modelId;
	auto playerVehicle = Globals::GetPlayerVehicle();

	if(vehicle == playerVehicle)
	{
		if(horn && sirenState == true)
		{
			if(Widgets::IsWidgetPressed(7))
			{
				auto timePressed = Widgets::m_Widgets[7].timePressed;

				if(timePressed > 100)
				{
					if(!hornState)
					{
						ToggleHorn(true);
						ToggleSiren(false);
					}
				}
			}
		}

		if(horn && hornState)
		{
			if(Widgets::IsWidgetJustReleased(7))
			{
				ToggleHorn(false);
				ToggleSiren(true);
			}
		}
	}

	CMatrix* pMatrix = camera->GetMatSA();
    CVector cameraPosition = pMatrix->pos;
	CVector vehiclePositon = vehicle->position;

	if(FixLoudSounds)
	{
		for(auto siren : sirens)
		{
			auto distanceFromCamera = DistanceBetween(vehiclePositon, cameraPosition);
			auto maxDistance = 10.0f;
		
			if(distanceFromCamera < maxDistance)
			{
				auto mult = distanceFromCamera/maxDistance;
				if(mult < 0.10f) mult = 0.10f;

				//Log::Level(LOG_LEVEL::LOG_DEEP_UPDATE) << "mult: " << mult << "| d: " << distanceFromCamera << std::endl;

				siren->SetVolume(mult);
			} else {
				siren->SetVolume(1.0f);
			}
		}
	}

	if(!SirenSystem::ModelIdHasSirenGroup(modelId)) return;

	/*
	auto siren = sirens[0];
	if(siren->GetState() == -1)
	{
		siren->Play();
	}
	*/
}

void SirenSystem::Destroy()
{
	if(horn)
	{
		horn->Stop();
		SoundSystem::UnloadStream(horn);
	}

	for(auto siren : sirens)
	{
		siren->Stop();
		SoundSystem::UnloadStream(siren);
	}

	if(radio)
	{
		radio->Stop();
		SoundSystem::UnloadStream(radio);
	}
}

void SirenSystem::ToggleHorn(bool enabled)
{
	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: ToggleHorn" << std::endl;

	if(!horn)
	{
		LoadAudios();
	}

	hornState = enabled;

	auto soundGroup = GetCurrentVehicleSoundGroup();

	if (enabled)
	{
		horn->Play();
	}
	else {
		horn->Pause();
	}
}

void SirenSystem::ToggleSiren(bool enabled)
{
	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: ToggleSiren " << (enabled ? "TRUE" : "FALSE") << std::endl;

	if(sirens.size() == 0)
	{
		LoadAudios();
	}

	sirenState = enabled;

	auto siren = sirens[currentSirenIndex];

	if (enabled)
	{
		siren->Play();
		prevSirenIndex = currentSirenIndex;
	}
	else {
		siren->Pause();
		prevSirenIndex = -1;
	}
}

void SirenSystem::ChangeSirenByOne()
{
	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: ChangeSirenByOne" << std::endl;

	int max = sirens.size();

	currentSirenIndex++;
	if (currentSirenIndex > max - 1)
	{
		currentSirenIndex = 0;
	}

	if (prevSirenIndex != -1)
	{
		sirens[prevSirenIndex]->Pause();
		prevSirenIndex = -1;
	}

	if (sirenState == true)
	{
		ToggleSiren(true);
	}
}

void SirenSystem::ToggleRadio(bool enabled)
{
	Log::Level(LOG_LEVEL::LOG_BOTH) << "SirenSystem: ToggleRadio " << (enabled ? "TRUE" : "FALSE") << std::endl;

	if(!radio)
	{
		LoadAudios();
	}

	if (enabled)
	{
		radio->Play();
	}
	else {
		radio->Pause();
	}
}