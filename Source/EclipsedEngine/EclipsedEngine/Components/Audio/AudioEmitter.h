#pragma once

#include <vector>
#include <functional>

#include "EclipsedEngine/Components/Component.h"

#include <Core/Math/Vector/Vector2.h>

#include "Assets/AssetManager.h"

namespace FMOD {
	class Channel;
}


namespace Eclipse
{
	namespace Editor
	{
		class InspectorWindow;
	}

	class ECL_API AudioEmitter : public Component
	{
		BASE_SELECTION(AudioEmitter, 0)

	/*public:
		void Awake() override;
		void OnDestroy() override;

		void Update() override;

	public:
		void SetSpatialMode(bool is3D);

		void Play();
		void Stop();

		void Resume();
		void Pause();

		void SetAudioClip(Assets::AudioClip clip);

		void SetVolume(float aVolume);
		float GetVolume() const;

	private:
		void InitAudio();
		void UpdateAudioPosition();*/

	public:
		SERIALIZED_FIELD(Assets::AudioClip, audioClip);

		SERIALIZED_FIELD_DEFAULT(bool, EnableSpatial, false);
		SERIALIZED_FIELD_DEFAULT(bool, playOnAwake, false);
		SERIALIZED_FIELD_DEFAULT(float, volume, 1.f);


	private:
		FMOD::Channel* channel = nullptr;
		bool isPlaying = false;
	};

}