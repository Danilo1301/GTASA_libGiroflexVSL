#include "AudioStream.h"
#include "Log.h"

extern IBASS* BASS;

AudioStream::AudioStream(std::string src)
{
    Log::file << "Loading audiostream '" << src << "'" << std::endl;

    unsigned flags = BASS_SAMPLE_SOFTWARE;
    //if (soundsys->bUseFPAudio) flags |= BASS_SAMPLE_FLOAT;

    if (!(streamInternal = BASS->StreamCreateFile(false, src.c_str(), 0, 0, flags)))
    {
        Log::file << "Loading audiostream '" << src << "' failed. Error code: " << BASS->ErrorGetCode() << std::endl;
    }
    else
    {
        Log::file << "Loading audiostream OK" << std::endl;
    }
}

void AudioStream::Play()
{
    BASS->ChannelPlay(streamInternal, true);
}

void AudioStream::Pause()
{
    BASS->ChannelPause(streamInternal);
}

void AudioStream::Stop()
{
    BASS->ChannelPause(streamInternal);
    BASS->ChannelSetPosition(streamInternal, 0, BASS_POS_BYTE);
}

void AudioStream::Resume()
{
    BASS->ChannelPlay(streamInternal, false);
}

void AudioStream::Loop(bool enable)
{
    BASS->ChannelFlags(streamInternal, enable ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
}

void AudioStream::SetVolume(float val)
{
    BASS->ChannelSetAttribute(streamInternal, BASS_ATTRIB_VOL, val);
}

void AudioStream::Destroy()
{
    if (streamInternal) BASS->StreamFree(streamInternal);
}

int AudioStream::GetState()
{
    switch (BASS->ChannelIsActive(streamInternal))
    {
    case BASS_ACTIVE_STOPPED:
    default:
        return -1;

    case BASS_ACTIVE_PLAYING:
    case BASS_ACTIVE_STALLED:
        return 1;

    case BASS_ACTIVE_PAUSED:
        return 2;
    };
}

void AudioStream::Set3DPosition(const CVector&)
{
    Log::file << "Unimplemented CAudioStream::Set3DPosition(const CVector&)" << std::endl;
}

void AudioStream::Set3DPosition(float, float, float)
{
    Log::file << "Unimplemented CAudioStream::Set3DPosition(float,float,float)" << std::endl;
}

void AudioStream::Link(CPlaceable*)
{
    Log::file << "Unimplemented CAudioStream::Link(CPlaceable*)" << std::endl;
}

void AudioStream::Process()
{

}