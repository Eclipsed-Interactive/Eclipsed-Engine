#include "AudioManager.h"

#include "fmod/fmod.hpp"

#include "CoreEngine/MainSingleton.h"

namespace Eclipse
{
	FMOD::System* AudioManager::mySystem;
	FMOD::Studio::System* AudioManager::myStudioSystem;

	void AudioManager::Init()
	{
		FMOD_RESULT result;

		result = FMOD::Studio::System::create(&myStudioSystem);
		result = myStudioSystem->initialize(
			512, 
			FMOD_STUDIO_INIT_NORMAL, 
			FMOD_INIT_NORMAL, 
			nullptr
		);

		myStudioSystem->getCoreSystem(&mySystem);

		MainSingleton::AddInstance<FMOD::System*>(mySystem);
		MainSingleton::AddInstance<FMOD::Studio::System*>(myStudioSystem);
	}

	void AudioManager::Update()
	{
		myStudioSystem->update();
	}

	void AudioManager::PlayAudio(FMOD::Sound* aSound, FMOD::Channel** aChannel)
	{
		mySystem->playSound(aSound, nullptr, false, aChannel);
	}
}
