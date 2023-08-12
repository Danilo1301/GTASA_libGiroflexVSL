#pragma once

#include "pch.h"

//https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams/blob/main/audiosystem.cpp

class AudioStream {
public:
	uint64_t streamInternal;
	static IBASS* _BASS;

	AudioStream(std::string src);

	void Play();
	void Pause();
	void Stop();
	void Resume();
	void Loop(bool enable);
	void SetVolume(float val);
	void Destroy();
	int GetState();
};