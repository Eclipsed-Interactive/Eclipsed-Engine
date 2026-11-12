#pragma once

#include "EclipsedEngine.Core.hpp"

#include <vector>
#include <functional>

#include "EclipsedEngine/Components/Component.h"

#include <Core/Math/Vector/Vector2.h>

#include "Assets/AssetManager.h"
#include <Assets/Assets/AudioAsset.h>

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
		void OnDestroy() override;

		void Update() override;

	public:
		void SetAudioClip(Assets::AudioClip clip);

		void Play();
		void Pause();
		void Stop();

	public:
		void SetVolume(float aVolume);
		float GetVolume() const;

		void SetPan(float aPan);
		float GetPan() const;

	public:
		void ApplyVolume();
		void ApplyAudioClip();
		void ApplyPan();

		void OnUnpause() override;
		void OnPause() override;

	public:
		[[SerializeField, OnChange(ApplyAudioClip)]]
		Assets::AudioClip AudioClip;

		[[SerializeField]] bool EnableSpatial = false;
		[[SerializeField]] bool playOnAwake = false;
		[[SerializeField, OnChange(ApplyVolume), Range(0.f, 1.f)]] float volume = 0.5f;
		[[SerializeField, OnChange(ApplyPan), Range(-1.f, 1.f)]] float pan = 0.f;

	private:
		Audio::IAudioChannel* channel = nullptr;

	private:
		bool myHasAudioClip = false;
		bool myIsPlaying = false;
	};

}