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


		/* void Update() override;

public:
	void SetSpatialMode(bool is3D);

	void Play();
	void Stop();

	void Resume();
	void Pause();

	void SetAudioClip(Assets::AudioClip clip);



private:
	void InitAudio();
	void UpdateAudioPosition();*/

	public:
		void ApplyVolume();
		void ApplyAudioClip();

	public:
		[[SerializeField, OnChange(ApplyAudioClip)]]
		Assets::AudioClip AudioClip;

		[[SerializeField]] bool EnableSpatial = false;
		[[SerializeField]] bool playOnAwake = false;
		[[SerializeField, OnChange(ApplyVolume), Step(0.01f), Range(0.f, 1.f)]] float volume = 1.f;

	private:
		Audio::IAudioChannel* channel = nullptr;

	private:
		bool myHasAudioClip = false;
	};

}