#include "AudioSource.h"

#include "AudioEngine/AudioManager.h"

namespace Eclipse
{
	void AudioSource::Awake()
	{
		if (playOnAwake) {
			Play();
		}
	}

	void AudioSource::OnDestroy() {
		channel->stop();
	}

	void AudioSource::Update()
	{
		SetVolume(volume);
	}

	void AudioSource::Play() {
		isPlaying = true;
		channel->setPaused(isPlaying);
		AudioManager::PlayAudio(audioClip->data->sound, &channel);
	}

	void AudioSource::Resume() {
		isPlaying = true;
		channel->setPaused(false);
	}

	void AudioSource::Pause() {
		isPlaying = false;
		channel->setPaused(true);
	}

	void AudioSource::SetAudioClip(AudioClip clip)
	{
		audioClip = clip;
	}

	void AudioSource::Stop() {
		channel->stop();
	}

	void AudioSource::SetVolume(float aVolume) {
		volume = aVolume;
		channel->setVolume(volume);
	}

	float AudioSource::GetVolume() const {
		return volume;
	}
}