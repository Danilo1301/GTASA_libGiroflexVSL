#pragma once

#include "pch.h"

#include "audiosystem.h"

class SirenGroup {
public:
	std::string id;

	std::string hornDir = "";
	std::vector<std::string> sirensDir;
	std::vector<int> compatibleVehicles;

	SirenGroup(std::string id)
	{
		this->id = id;
	}

	bool IsVehicleModelCopatible(int modelId)
	{
		for (auto compatibleVehicle : compatibleVehicles)
		{
			if (compatibleVehicle == modelId)
			{
				return true;
			}
		}
		return false;
	}
};

class SirenSystem {
public:
	static std::string m_DefaultGroupId;
	static std::map<std::string, SirenGroup*> m_SirenGroups;
	static bool FixLoudSounds;

	static void Load();
	static void LoadConfig();
	static void LoadVehicles();

    static SirenGroup* GetSirenGroupById(std::string id);
    static SirenGroup* GetDefaultSirenGroup();
    static SirenGroup* GetSirenGroupForModelId(int modelId);
    static bool ModelIdHasSirenGroup(int modelId);
    static int GetCurrentVehicleModelId();
    static SirenGroup* GetCurrentVehicleSoundGroup();

	//

	int hVehicle = 0;

	bool hornState = false;
	bool sirenState = false;
	int currentSirenIndex = 0;

	CAudioStream* horn = NULL;
	std::vector<CAudioStream*> sirens;

	CAudioStream* radio = NULL;

	int prevSirenIndex = 0; //to fix

	SirenSystem(int hVehicle);

	void LoadAudios();

	void Init();
	void Update(int dt);
	void Destroy();

	void ToggleHorn(bool enabled);
	void ToggleSiren(bool enabled);
	void ChangeSirenByOne();

	void ToggleRadio(bool enabled);
};