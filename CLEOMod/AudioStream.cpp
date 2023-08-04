#include "AudioStream.h"
#include "Log.h"

IBASS* AudioStream::_BASS = NULL;

AudioStream::AudioStream(std::string src)
{
    Log::file << "Loading audiostream '" << src << "'" << std::endl;

    unsigned flags = BASS_SAMPLE_SOFTWARE;
    //if (soundsys->bUseFPAudio) flags |= BASS_SAMPLE_FLOAT;

    if (!_BASS)
    {
        Log::file << "BASS undefined" << std::endl;
        return;
    }

    if (!(streamInternal = _BASS->StreamCreateFile(false, src.c_str(), 0, 0, flags)))
    {
        Log::file << "Loading audiostream failed. Error code: " << _BASS->ErrorGetCode() << std::endl;
    }
    else
    {
        Log::file << "Loading audiostream OK" << std::endl;
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