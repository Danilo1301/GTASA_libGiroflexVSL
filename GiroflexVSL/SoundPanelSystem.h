#include "pch.h"

#include "AudioStream.h"

class SoundGroup {
public:
	std::string id;

	AudioStream* horn = NULL;
	std::vector<AudioStream*> sirens;
	std::vector<int> compatibleVehicles;

	SoundGroup(std::string id)
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

class SoundPanelSystem {
public:
	static std::string defaultGroupId;
	static std::map<std::string, SoundGroup*> soundGroups;
	static SoundGroup* prevSoundGroup;
	static AudioStream* prevSiren;
	static bool sirenState;
	static bool hornState;

	static int currentSirenIndex;

	static void Load();
	static void LoadConfig();
	static void LoadVehicles();

	static SoundGroup* GetSoundGroupById(std::string id);
	static SoundGroup* GetDefaultSoundGroup();
	static SoundGroup* GetSoundGroupForModelId(int modelId);

	static int GetCurrentVehicleModelId();
	static SoundGroup* GetCurrentVehicleSoundGroup();

	static void Update(int dt);

	static void ToggleHorn(bool enabled);
	static void ToggleSiren(bool enabled);
	static void ChangeSirenByOne();
};