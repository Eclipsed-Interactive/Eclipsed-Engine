#include "AudioEmitter.h"

#include "AudioEngine/AudioManager.h"

namespace Eclipse
{
	void AudioEmitter::Awake()
	{
		if (playOnAwake) {
			Play();
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
}