#include "AudioEmitter.h"

#include "Audio/AudioPluginLoader.h"
#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/Audio/AudioListener.h"

#include <AssetSystem/Assets.h>

namespace Eclipse
{
	void AudioEmitter::Awake()
	{
		Audio::IAudioBackend* backend = Audio::AudioManager::GetBackend();

		if (!backend)
			return;

		channel = backend->CreateAudioChannel();

		if (!channel)
			return;

		ApplyAudioClip();
		ApplyVolume();
		ApplyPan();

		if (playOnAwake && audioClip.IsReady())
			Play();
	}

	void AudioEmitter::OnDestroy()
	{
		if (!audioClip.IsReady() || !channel)
			return;

		myIsPlaying = false;
		channel->Stop();

		delete channel;
		channel = nullptr;
	}

	void AudioEmitter::Update()
	{
		if (!audioClip.IsReady() || !channel)
			return;

		//ApplyVolume();
	}

	void AudioEmitter::SetAudioClip(AudioClip aClip)
	{
		audioClip = aClip;
		channel->SetSound(audioClip.GetAudio());

		ApplyAudioClip();
	}

	void AudioEmitter::SetVolume(float aVolume)
	{
		volume = aVolume;
		
		if (!channel)
			return;

		channel->SetVolume(volume);
	}

	void AudioEmitter::Play()
	{
		if (!audioClip.IsReady() || !channel)
			return;

		myIsPlaying = true;
		channel->Play();
	}

	void AudioEmitter::Pause()
	{
		if (!audioClip.IsReady() || !channel)
			return;

		myIsPlaying = false;
		channel->Pause();
	}

	void AudioEmitter::Stop()
	{
		if (!audioClip.IsReady() || !channel)
			return;

		myIsPlaying = false;
		channel->Stop();
	}

	float AudioEmitter::GetVolume() const
	{
		return volume;
	}

	void AudioEmitter::SetPan(float aPan)
	{
		pan = aPan;

		if (!channel)
			return;

		channel->SetPan(pan);
	}

	float AudioEmitter::GetPan() const
	{
		return pan;
	}

	void AudioEmitter::ApplyVolume()
	{
		if (!audioClip.IsReady() || !channel)
			return;

		//channel->Volume = volume; // Linear
		//channel->Volume = volume * volume * volume; // Cubic
		//channel->Volume = std::powf(10, ((-60 * (1 - volume)) / 20)); // Decibel
		channel->Volume = (std::exp(6.9f * volume) - 1.f) / (std::exp(6.9f) - 1.f); // Exponential
	}

	void AudioEmitter::ApplyAudioClip()
	{
		if (!channel)
			return;

		channel->Stop();

		if (!audioClip.IsReady())
			return;

		channel->SetSound(audioClip.GetAudio());

		if (playOnAwake)
			channel->Play();
	}

	void AudioEmitter::ApplyPan()
	{
		if (!audioClip.IsReady() || !channel)
			return;

		channel->Pan = pan;
	}

	void AudioEmitter::OnUnpause()
	{
		if (myIsPlaying)
			channel->Play();
	}


	void AudioEmitter::OnPause()
	{
		if (myIsPlaying)
			channel->Pause();
	}
}