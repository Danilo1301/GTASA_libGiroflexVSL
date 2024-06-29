#include "SoundSystem.h"
#include "Log.h"
#include "ModConfig.h"
#include "audiosystem.h"

extern IBASS* BASS;
extern CSoundSystem* soundsys;

CAudioStream* SoundSystem::LoadStream(std::string src, bool in3d)
{
    return soundsys->LoadStream(src.c_str(), in3d);
}

CAudioStream* SoundSystem::LoadStreamFromAudiosFolder(std::string src, bool in3d)
{
    std::string audiosPath = ModConfig::GetConfigFolder() + "/audios/";

    auto result = LoadStream(audiosPath + src, in3d);

    return result;
}

void SoundSystem::UnloadStream(CAudioStream* stream)
{
    soundsys->UnloadStream(stream);
}

CAudioStream* SoundSystem::PlayStream(std::string src, bool loop)
{
    if(!BASS)
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "BASS is not loaded: can't play stream" << std::endl;
        return NULL;
    }

    CAudioStream* audioStream = LoadStream(src, false);

    if(audioStream)
    {
        if(loop) audioStream->Loop(true);
        audioStream->Play();
    }

    return audioStream;
}

CAudioStream* SoundSystem::PlayStreamFromAudiosFolder(std::string src, bool loop)
{
    std::string audiosPath = ModConfig::GetConfigFolder() + "/audios/";

    return PlayStream(audiosPath + src, loop);
}

/*
TODO: use the ModConfig::FindRandomFileVariations
*/
CAudioStream* SoundSystem::PlayStreamFromAudiosFolderWithRandomVariation(std::string src, bool loop)
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "SoundSystem: PlayStreamWithRandomVariation '" << src << "'" << std::endl;

    std::string audiosPath = ModConfig::GetConfigFolder() + "/audios/";

    int i = 1;
    bool canLoop = true;
    do
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "Trying to find audio variation: " << i << std::endl;

        auto audioFile = audiosPath + src + std::to_string(i) + ".wav";

        if(ModConfig::FileExists(audioFile))
        {
            Log::Level(LOG_LEVEL::LOG_BOTH) << audioFile << " found" << std::endl;

            i++;
        } else {
            Log::Level(LOG_LEVEL::LOG_BOTH) << audioFile << " not found" << std::endl;
            canLoop = false;
        }
    } while (canLoop);
    
    int numVariations = i - 1;

    Log::Level(LOG_LEVEL::LOG_BOTH) << "Found " << numVariations << " variations" << std::endl;

    auto newSrc = audiosPath + src + std::to_string(GetRandomNumber(1, numVariations)) + ".wav";

    return PlayStream(newSrc, loop);
}