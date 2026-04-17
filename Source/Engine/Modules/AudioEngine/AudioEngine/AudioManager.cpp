#include "AudioManager.h"

#include "fmod/fmod.hpp"

#include "CoreEngine/MainSingleton.h"

#include <filesystem>
#include <iostream>

namespace Eclipse
{
	FMOD::System* AudioManager::mySystem;
	FMOD::Studio::System* AudioManager::myStudioSystem;

	void AudioManager::Init()
	{
        FMOD_RESULT result;

        result = FMOD::Studio::System::create(&myStudioSystem);
        if (result != FMOD_OK) {
            return;
        }

        result = myStudioSystem->initialize(
            512,
            FMOD_STUDIO_INIT_NORMAL,
            FMOD_INIT_3D_RIGHTHANDED,
            nullptr
        );
        if (result != FMOD_OK) {
            return;
        }


        result = FMOD::System_Create(&mySystem);
        if (result != FMOD_OK) {
            return;
        }

        result = mySystem->setSoftwareFormat(48000, FMOD_SPEAKERMODE_STEREO, 0);
        if (result != FMOD_OK) {
            return;
        }

        result = mySystem->init(
            512, FMOD_INIT_3D_RIGHTHANDED, nullptr
        );
        if (result != FMOD_OK) {
            return;
        }




        result = mySystem->set3DSettings(1.0f, 1.0f, 1.0f);
        if (result != FMOD_OK) {
            return;
        }





        { // Make sure the system is correctly setup.
            unsigned version = 0;
            result = mySystem->getVersion(&version);

            if (result != FMOD_OK) {
                return;
            }
        }


		MainSingleton::AddInstance<FMOD::System*>(mySystem);
		MainSingleton::AddInstance<FMOD::Studio::System*>(myStudioSystem);
	}

	void AudioManager::Update()
	{
		myStudioSystem->update();
        mySystem->update();
	}

	void AudioManager::PlayAudio(FMOD::Sound* aSound, FMOD::Channel** aChannel)
	{
		mySystem->playSound(aSound, nullptr, false, aChannel);
	}
}
