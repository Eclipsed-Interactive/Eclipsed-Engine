#pragma once

#include <vector>
#include <functional>

#include "EclipsedEngine/Components/Component.h"

#include <CoreEngine/Math/Vector/Vector2.h>

#include "AssetEngine/AssetManager.h"

namespace FMOD {
	class Channel;
}


namespace Eclipse
{
	namespace Editor
	{
		class InspectorWindow;
	}

	class ECLIPSED_API AudioEmitter : public Component
	{
		BASE_SELECTION(AudioEmitter, 0)

	public:
		void Awake() override;
		void OnDestroy() override;

		void Update() override;

	public:
		void Play();
		void Stop();

		void Resume();
		void Pause();

		void SetAudioClip(Assets::Audio clip);

		void SetVolume(float aVolume);
		float GetVolume() const;

	private:
		void UpdateAudioPosition();

	public:
		SERIALIZED_FIELD(Assets::Audio, audioClip);

		SERIALIZED_FIELD_DEFAULT(bool, EnableSpatial, true);
		SERIALIZED_FIELD_DEFAULT(bool, playOnAwake, false);
		SERIALIZED_FIELD_DEFAULT(float, volume, 1.f);


	private:
		FMOD::Channel* channel;
		bool isPlaying = false;
	};

}