#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "EclipsedEngine.Core.hpp"
namespace FMOD::Studio {
	class EventDescription;
	class EventInstance;
}

namespace Eclipse {
	class ECL_API AudioEvent : public Component
	{
		MAKE_REFLECTABLE(AudioEvent);
		REFLECTION_FLAG(COMPONENT);

		BASE_SELECTION(AudioEvent, 1)

	public:
		void Awake() override;
		void OnDestroy() override;

		void Update() override;

	public:
		void Play();
		void Stop();

		void Resume();
		void Pause();

		void SetAudioEvent(const char* anEvent);

		void SetVolume(float aVolume);
		float GetVolume() const;

	public:
		[[SerializeField]] std::string event;

		[[SerializeField]] bool playOnAwake = false;
		[[SerializeField]] float volume = 1.f;

	private:
		bool isPlaying = false;
		FMOD::Studio::EventDescription* desc;
		FMOD::Studio::EventInstance* instance;
	};
}