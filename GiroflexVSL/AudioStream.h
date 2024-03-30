#pragma once

//https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams/blob/main/audiosystem.cpp

#include "pch.h"

class AudioStream {
public:
	uint64_t streamInternal;

	AudioStream(std::string src);

	void Play();
	void Pause();
	void Stop();
	void Resume();
	void Loop(bool enable);
	void SetVolume(float val);
	void Destroy();
	int GetState();

	virtual void Set3DPosition(const CVector& pos);
	virtual void Set3DPosition(float x, float y, float z);
	virtual void Link(CPlaceable* placeable = NULL);
	virtual void Process();
};