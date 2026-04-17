#pragma once

#include "EclipsedEngine/Components/Component.h"

namespace FMOD::Studio {
	class EventDescription;
	class EventInstance;
}

namespace Eclipse {
	class ECLIPSED_API AudioEvent : public Component
	{
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
		SERIALIZED_FIELD(std::string, event);

		SERIALIZED_FIELD_DEFAULT(bool, playOnAwake, false);
		SERIALIZED_FIELD_DEFAULT(float, volume, 1.f);

	private:
		bool isPlaying = false;
		FMOD::Studio::EventDescription* desc;
		FMOD::Studio::EventInstance* instance;
	};
}