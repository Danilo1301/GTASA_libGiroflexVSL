#pragma once

//https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams/blob/main/audiosystem.cpp

#include "pch.h"

#include "AudioStream.h"
#include "AudioStream3D.h"

#include <set>

class SoundSystem {
public:
	static std::set<AudioStream*> streams;

	static bool Init();
	static void Update();
	static AudioStream* LoadStream(std::string src, bool in3d);
};