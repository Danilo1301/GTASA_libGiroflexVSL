#pragma once

//https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams/blob/main/audiosystem.cpp

#include "pch.h"

#include "audiosystem.h"

#include <set>

class SoundSystem {
public:
	static CAudioStream* LoadStream(std::string src, bool in3d);
	static CAudioStream* LoadStreamFromAudiosFolder(std::string src, bool in3d);
	static void UnloadStream(CAudioStream* stream);

	static CAudioStream* PlayStream(std::string src, bool loop);
	static CAudioStream* PlayStreamFromAudiosFolder(std::string src, bool loop);
	static CAudioStream* PlayStreamFromAudiosFolderWithRandomVariation(std::string src, bool loop);
};