#pragma once

#include <vector>
#include <functional>

#include "EclipsedEngine/Components/Component.h"
#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"
#include <Core/Math/Vector/Vector2.h>

#include "Assets/AssetManager.h"

namespace Eclipse::Audio {
	class IAudioChannel;
}


namespace Eclipse
{
	namespace Editor
	{
		class InspectorWindow;
	}

	class ECL_API AudioEmitter : public Component
	{
		MAKE_REFLECTABLE(AudioEmitter);
		REFLECTION_FLAG(COMPONENT);

		BASE_SELECTION(AudioEmitter, 0)

	public:
		void Awake() override;
		/*	void OnDestroy() override;

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
		[[SerializeField]]
		Assets::AudioClip AudioClip;

		[[SerializeField]] bool EnableSpatial = false;
		[[SerializeField]] bool playOnAwake = false;
		[[SerializeField]] float volume = 1.f;

	private:
		Audio::IAudioChannel* channel;
	};

}