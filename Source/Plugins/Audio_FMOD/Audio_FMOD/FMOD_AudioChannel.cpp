#include "FMOD_AudioChannel.h"

#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"

#include "FMOD_AudioBackend.h"
#include "FMOD_Sound.h"

namespace Eclipse::Audio
{
	void FMOD_AudioChannel::Play()
	{		
		if (!myChannel)
			return;

		myChannel->setPaused(false);
	}

	void FMOD_AudioChannel::Pause()
	{
		if (!myChannel)
			return;

		myChannel->setPaused(true);
	}

	void FMOD_AudioChannel::Stop()
	{
		if (!myChannel)
			return;

		myChannel->stop();
	}

	void FMOD_AudioChannel::SetSound(ISound* aSound)
	{
		FMOD::Sound* fSound = static_cast<FMOD::Sound*>(aSound->GetSound());
		if (!fSound) return;

		FMOD_Backend::system->playSound(fSound, nullptr, false, &myChannel);
		myChannel->setPaused(true);
	}


	void FMOD_AudioChannel::ApplyVolume()
	{
		if (!myChannel)
			return;

		myChannel->setVolume(GetVolume());
	}

	//void Pause() override;
	//void Resume() override;
	//void Stop() override;
	//PlayMode GetPlayMode() const override;
	//bool IsValid() const override;

	//// Mode
	//void SetMode(AudioMode mode) override;
	//AudioMode GetMode() const override;

	//// Mixing
	//void ApplyVolume() override;
	//void SetPitch(float pitch) override;
	//float GetPitch() const override;
	//void SetPan(float pan) = 0; // -1 left 1 roverridet
	//void SetMuted(bool muted) override;
	//bool IsMuted() const override;
	//void FadeTo(float volume, float seconds) override;

	//// Time
	//void SetLoopCount(int count) = 0; // -1 infioverridee
	//void SetPosition(unsigned ms) override;
	//unsigned GetPosition() const override;
	//unsigned GetLength() const override;

	//// 3D
	//void Set3DAttributes(const Math::Vector2f& position, const Math::Vector2f& velocity) override;
	//void Set3DMinMaxDistance(float minDistance, float maxDistance) override;
}