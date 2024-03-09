#include "AudioStream3D.h"
#include "Log.h"

extern IBASS* BASS;

AudioStream3D::AudioStream3D(std::string src) : AudioStream(src)
{
    Log::file << "Loading audiostream 3d '" << src << "'" << std::endl;

    unsigned flags = BASS_SAMPLE_SOFTWARE;
    //if (soundsys->bUseFPAudio) flags |= BASS_SAMPLE_FLOAT;

    if (!(streamInternal = BASS->StreamCreateFile(false, src.c_str(), 0, 0, flags)))
    {
        Log::file << "Loading audiostream 3d '" << src << "' failed. Error code: " << BASS->ErrorGetCode() << std::endl;
    }
    else
    {
        Log::file << "Loading audiostream 3d OK" << std::endl;
    }
}


void AudioStream3D::Set3DPosition(const CVector& pos)
{
    position.x = pos.y;
    position.y = pos.z;
    position.z = pos.x;
    link = NULL;
    BASS->ChannelSet3DPosition(streamInternal, &position, NULL, NULL);
}

void AudioStream3D::Set3DPosition(float x, float y, float z)
{
    position.x = y;
    position.y = z;
    position.z = x;
    link = NULL;
    BASS->ChannelSet3DPosition(streamInternal, &position, NULL, NULL);
}

void AudioStream3D::Link(CPlaceable* placeable)
{
    link = placeable;
}

void AudioStream3D::Process()
{
    if (link)
    {
        CVector* pVec = link->GetPosSA();
        position.x = pVec->y;
        position.y = pVec->z;
        position.z = pVec->x;
    }
    BASS->ChannelSet3DPosition(streamInternal, &position, NULL, NULL);
    //Log::file << "AudioStream3D pos: " << position.x << ", " << position.y << ", " << position.z << std::endl;
}
