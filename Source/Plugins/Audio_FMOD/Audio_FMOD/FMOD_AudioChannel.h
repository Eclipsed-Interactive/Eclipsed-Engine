#pragma once

#include <Audio/IAudioChannel.h>

namespace FMOD
{
	class Channel;
	class Sound;
}

namespace Eclipse::Audio
{
	class FMOD_AudioChannel : public IAudioChannel
	{
	public:
		// Playback
		void Play(ISound* aSound) override;
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

	private:
		FMOD::Channel* myChannel = nullptr;
	};
}