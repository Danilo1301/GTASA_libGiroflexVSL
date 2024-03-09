#pragma once

//https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams/blob/main/audiosystem.cpp

#include "pch.h"

#include "AudioStream.h"

class AudioStream3D : public AudioStream {
public:
	AudioStream3D(std::string src);
protected:
	CPlaceable* link;
	BASS_3DVECTOR  position;

	virtual void Set3DPosition(const CVector& pos);
	virtual void Set3DPosition(float x, float y, float z);
	virtual void Link(CPlaceable* placeable = NULL);
	virtual void Process();
};