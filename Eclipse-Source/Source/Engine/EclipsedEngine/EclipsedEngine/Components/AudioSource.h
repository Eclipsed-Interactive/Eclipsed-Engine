#pragma once

#include <vector>
#include <functional>

#include "EclipsedEngine/Components/Component.h"

#include <CoreEngine/Math/Vector/Vector2.h>

#include "AssetEngine/Assets/AudioClip.h"

namespace FMOD {
	class Channel;
}


namespace Eclipse
{
	namespace Editor
	{
		class InspectorWindow;
	}

	class ECLIPSED_API AudioSource : public Component
	{
		BASE_SELECTION(AudioSource, 0)


	public:
		void Awake() override;
		void OnDestroy() override;

		void Update() override;

	public:
		void Play();
		void Stop();

		void Resume();
		void Pause();

		void SetAudioClip(AudioClip clip);

		void SetVolume(float aVolume);
		float GetVolume() const ;

	public:
		SERIALIZED_FIELD(AudioClip, audioClip);
		
		SERIALIZED_FIELD_DEFAULT(bool, playOnAwake, false);
		SERIALIZED_FIELD_DEFAULT(float, volume, 1.f);


	private:
		FMOD::Channel* channel;
		bool isPlaying = false;
	};

}