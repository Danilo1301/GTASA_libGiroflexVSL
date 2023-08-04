#pragma once

#include "pch.h"

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
};