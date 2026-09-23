#include "AudioEmitter.h"

//#include "Audio/AudioManager.h"
#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/Audio/AudioListener.h"

namespace Eclipse
{
	/*void AudioEmitter::Awake()
	{
		if (playOnAwake)
			Play();

		Transform2D* trans = gameObject->transform;

		trans->AddFunctionToRunOnDirtyUpdate(this,
			[this]()
			{
				this->UpdateAudioPosition();
			}
		);
	}

	void AudioEmitter::OnDestroy()
	{
		if (channel)
		{
			channel->stop();
			channel = nullptr;
		}
	}

	void AudioEmitter::Update()
	{
		if (!channel)
			return;

		SetVolume(volume);
	}

	void AudioEmitter::SetSpatialMode(bool is3D)
	{
		if (!channel)
			return;

		if (is3D)
		{
			channel->setMode(
				FMOD_3D |
				FMOD_3D_WORLDRELATIVE |
				FMOD_3D_INVERSEROLLOFF
			);
		}
		else
		{
			channel->setMode(FMOD_2D);
		}
	}

	void AudioEmitter::Play()
	{
		if (!audioClip.IsValid())
			return;

		AudioManager::PlayAudio(
			audioClip->dataPtr->sound,
			&channel
		);

		if (!channel)
			return;

		InitAudio();

		channel->setPosition(0, FMOD_TIMEUNIT_MS);

		isPlaying = true;

		UpdateAudioPosition();
		SetSpatialMode(EnableSpatial);
		SetVolume(volume);

		channel->setPaused(false);
	}

	void AudioEmitter::Resume()
	{
		if (!channel)
			return;

		isPlaying = true;
		channel->setPaused(false);
	}

	void AudioEmitter::Pause()
	{
		if (!channel)
			return;

		isPlaying = false;
		channel->setPaused(true);
	}

	void AudioEmitter::SetAudioClip(Assets::AudioClip clip)
	{
		audioClip = clip;
	}

	void AudioEmitter::Stop()
	{
		if (!channel)
			return;

		channel->stop();
		channel = nullptr;

		isPlaying = false;
	}

	void AudioEmitter::SetVolume(float aVolume)
	{
		if (!channel)
			return;

		AudioListener* listener = AudioListener::GetListener();

		float audioAttenuation = 1.f;

		if (EnableSpatial && listener)
		{
			Math::Vector2f p =
				gameObject->transform->GetPosition();

			Math::Vector2f l =
				listener->gameObject->transform->GetPosition();

			float dx = p.x - l.x;
			float dy = p.y - l.y;

			float dist = sqrt(dx * dx + dy * dy);

			float t = dist / 20.f;
			t = std::clamp(t, 0.0f, 1.0f);

			audioAttenuation =
				1.0f - (t * t * (3.0f - 2.0f * t));
		}

		volume = aVolume;
		channel->setVolume(audioAttenuation * volume);
	}

	float AudioEmitter::GetVolume() const
	{
		return volume;
	}

	void AudioEmitter::InitAudio()
	{
		if (!channel)
			return;

		channel->setChannelGroup(
			AudioManager::GetBus(AudioBus::Master)
		);

		SetSpatialMode(EnableSpatial);

		channel->set3DMinMaxDistance(2.0f, 20.0f);
	}

	void AudioEmitter::UpdateAudioPosition()
	{
		if (!channel)
			return;

		Transform2D* trans = gameObject->transform;

		Math::Vector2f ePos = trans->GetPosition();

		FMOD_VECTOR pos =
		{
			ePos.x,
			ePos.y,
			1.f
		};

		FMOD_VECTOR vel =
		{
			0.f,
			0.f,
			0.f
		};

		channel->set3DAttributes(&pos, &vel);
	}*/
}