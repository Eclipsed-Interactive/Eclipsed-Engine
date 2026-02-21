#include "AudioManager.h"

#include "fmod/fmod.hpp"

#include "CoreEngine/MainSingleton.h"

namespace Eclipse
{
    FMOD::System* AudioManager::mySystem;

	void AudioManager::Init() 
	{
        FMOD_RESULT result;

        result = FMOD::System_Create(&mySystem);
        result = mySystem->init(512, FMOD_INIT_NORMAL, nullptr);

        MainSingleton::AddInstance<FMOD::System*>(mySystem);
	}

	void AudioManager::Update()
	{
        mySystem->update();
	}
    
    void AudioManager::PlayAudio(FMOD::Sound* aSound, FMOD::Channel** aChannel)
    {
        mySystem->playSound(aSound, nullptr, false, aChannel);
    }


    void AudioManager::CreateAudioFromPath(const char* path, FMOD::Sound* aSound)
    {
        FMOD_MODE mode = FMOD_OPENMEMORY | FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE;

        mySystem->createSound(path, mode, nullptr, &aSound);
    }

    void AudioManager::CreateAudioFromMemory(const char* data, size_t size, FMOD::Sound* aSound)
    {
        FMOD_MODE mode = true ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;

        FMOD_CREATESOUNDEXINFO exinfo = {};
        exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        exinfo.length = size;

        mySystem->createSound((const char*)data, FMOD_OPENMEMORY | mode, &exinfo, &aSound);
    }

    //FMOD::Sound* AudioManager::CreateAudioFromPcmFile(const PCM& pcmFile)
    //{
    //    FMOD::Sound* sound = nullptr;
    //    FMOD_CREATESOUNDEXINFO exInfo = {};

    //    exInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    //    exInfo.length = pcmFile.bytes.size();
    //    exInfo.numchannels = pcmFile.data.channels;
    //    exInfo.defaultfrequency = (int)pcmFile.data.frequency;

    //    if (pcmFile.data.bits == 8)
    //        exInfo.format = FMOD_SOUND_FORMAT_PCM8;
    //    else if (pcmFile.data.bits == 16)
    //        exInfo.format = FMOD_SOUND_FORMAT_PCM16;
    //    else if (pcmFile.data.bits == 32)
    //        exInfo.format = FMOD_SOUND_FORMAT_PCM32;

    //    FMOD_MODE mode = FMOD_OPENMEMORY | FMOD_OPENRAW | FMOD_CREATESAMPLE;


    //    FMOD_RESULT result = mySystem->createSound(pcmFile.bytes.data(), mode, &exInfo, &sound);

    //    if (result != FMOD_OK)
    //    {
    //        return nullptr;
    //    }

    //    return sound;
    //}
}
