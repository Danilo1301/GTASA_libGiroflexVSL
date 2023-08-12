#include "AudioStream.h"
#include "Log.h"

IBASS* AudioStream::_BASS = NULL;

AudioStream::AudioStream(std::string src)
{
    //Log::file << "Loading audiostream '" << src << "'" << std::endl;

    unsigned flags = BASS_SAMPLE_SOFTWARE;
    //if (soundsys->bUseFPAudio) flags |= BASS_SAMPLE_FLOAT;

    if (!_BASS)
    {
        Log::file << "BASS undefined" << std::endl;
        return;
    }

    if (!(streamInternal = _BASS->StreamCreateFile(false, src.c_str(), 0, 0, flags)))
    {
        Log::file << "Loading audiostream '" << src << "' failed. Error code: " << _BASS->ErrorGetCode() << std::endl;
    }
    else
    {
        //Log::file << "Loading audiostream OK" << std::endl;
    }
}

void AudioStream::Play()
{
    _BASS->ChannelPlay(streamInternal, true);
}

void AudioStream::Pause()
{
    _BASS->ChannelPause(streamInternal);
}

void AudioStream::Stop()
{
    _BASS->ChannelPause(streamInternal);
    _BASS->ChannelSetPosition(streamInternal, 0, BASS_POS_BYTE);
}

void AudioStream::Resume()
{
    _BASS->ChannelPlay(streamInternal, false);
}

void AudioStream::Loop(bool enable)
{
    _BASS->ChannelFlags(streamInternal, enable ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
}

void AudioStream::SetVolume(float val)
{
    _BASS->ChannelSetAttribute(streamInternal, BASS_ATTRIB_VOL, val);
}

void AudioStream::Destroy()
{
    if (streamInternal) _BASS->StreamFree(streamInternal);
}

int AudioStream::GetState()
{
    switch (_BASS->ChannelIsActive(streamInternal))
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