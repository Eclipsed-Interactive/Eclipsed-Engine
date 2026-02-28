#include "AudioEmitter.h"

#include "AudioEngine/AudioManager.h"
#include "EclipsedEngine/Components/Transform2D.h"

namespace Eclipse
{
	void AudioEmitter::Awake()
	{
		if (playOnAwake) {
			Play();
		}

		if (EnableSpatial) {
			Transform2D* trans = gameObject->transform;
			trans->AddFunctionToRunOnDirtyUpdate(
				[this]() {
					this->UpdateAudioPosition();
				}
			);

			UpdateAudioPosition();
		}
	}

	void AudioEmitter::OnDestroy() {
		channel->stop();
	}

	void AudioEmitter::Update()
	{
		SetVolume(volume);
	}

	void AudioEmitter::Play() {
		isPlaying = true;
		channel->setPaused(isPlaying);
		AudioManager::PlayAudio(audioClip->data->sound, &channel);
	}

	void AudioEmitter::Resume() {
		isPlaying = true;
		channel->setPaused(false);
	}

	void AudioEmitter::Pause() {
		isPlaying = false;
		channel->setPaused(true);
	}

	void AudioEmitter::SetAudioClip(AudioClip clip)
	{
		audioClip = clip;

		channel->setMode(FMOD_3D_LINEARROLLOFF);
		channel->set3DMinMaxDistance(1.0f, 1000.0f);
	}

	void AudioEmitter::Stop() {
		channel->stop();
	}

	void AudioEmitter::SetVolume(float aVolume) {
		volume = aVolume;
		channel->setVolume(volume);
	}

	float AudioEmitter::GetVolume() const {
		return volume;
	}

	void AudioEmitter::UpdateAudioPosition()
	{
		Transform2D* trans = gameObject->transform;
		Math::Vector2f ePos = trans->GetPosition();

		FMOD_VECTOR pos = { ePos.x, ePos.y, 0.f };
		channel->set3DAttributes(&pos, nullptr);
	}
}