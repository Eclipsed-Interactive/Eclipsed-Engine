#include "FMOD_AudioChannel.h"

#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"

#include "FMOD_AudioBackend.h"
#include "FMOD_Sound.h"

namespace Eclipse::Audio
{
	void FMOD_AudioChannel::Play(ISound* aSound)
	{		
		FMOD::Sound* fSound = static_cast<FMOD::Sound*>(aSound->GetSound());
		if (!fSound) return;

		FMOD_Backend::system->playSound(fSound, nullptr, false, &myChannel);
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