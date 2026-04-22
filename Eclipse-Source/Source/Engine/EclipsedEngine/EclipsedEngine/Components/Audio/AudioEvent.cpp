#include "AudioEvent.h"

#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"

namespace Eclipse
{
	void AudioEvent::Awake()
	{
		if (event->empty()) return;

		SetAudioEvent(event->c_str());

		if (playOnAwake && instance) {
			instance->start();
		}
	}

	void AudioEvent::OnDestroy()
	{
		if (!instance) return;

		instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		instance->keyOff();
		instance->release();

		FMOD::Studio::System* studio = MainSingleton::GetRaw<FMOD::Studio::System*>();
		if (studio)
		{
			studio->update();
		}

		instance = nullptr;
	}

	void AudioEvent::Update()
	{
		SetVolume(volume);
	}

	void AudioEvent::Play()
	{
		instance->start();
	}

	void AudioEvent::Stop()
	{
		instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}

	void AudioEvent::Resume()
	{
		isPlaying = true;
		instance->setPaused(&isPlaying);
	}

	void AudioEvent::Pause()
	{
		isPlaying = false;
		instance->setPaused(&isPlaying);
	}

	void AudioEvent::SetAudioEvent(const char* anEvent)
	{
		FMOD::Studio::System* studio = MainSingleton::GetRaw<FMOD::Studio::System*>();

		if (instance)
		{
			instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
			instance->release();
			instance = nullptr;
		}

		FMOD_RESULT result = studio->getEvent(anEvent, &desc);
		if (result == FMOD_OK && desc)
		{
			result = desc->createInstance(&instance);
			if (result != FMOD_OK)
			{
				instance = nullptr;
			}
		}
	}

	void AudioEvent::SetVolume(float aVolume)
	{
		volume = aVolume;
		instance->setVolume(aVolume);
	}

	float AudioEvent::GetVolume() const
	{
		return volume;
	}
}