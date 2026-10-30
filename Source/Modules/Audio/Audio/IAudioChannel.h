#pragma once

#include <Core/Math/Vector/Vector2.h>

#include "IAudioChannel_Volume.h"
#include <Audio/ISound.h>

namespace Eclipse::Audio
{
	enum class AudioMode : unsigned int
	{
		None = 0,
		Mode3D = 1 << 0,
		WorldRelative = 1 << 1,
		HeadRelative = 1 << 2,
		InverseRolloff = 1 << 3,
		LinearRolloff = 1 << 4,
		Loop = 1 << 5,
	};

	constexpr AudioMode operator|(AudioMode a, AudioMode b) { return AudioMode(uint32_t(a) | uint32_t(b)); }
	constexpr AudioMode operator&(AudioMode a, AudioMode b) { return AudioMode(uint32_t(a) & uint32_t(b)); }
	constexpr bool HasFlag(AudioMode mode, AudioMode flag) { return (mode & flag) != AudioMode::None; }

	enum class PlayMode
	{
		NotStarted, Playing, Paused, Stopped
	};

	class IAudioChannel : public AudioVolume
	{
	public:
		virtual ~IAudioChannel() = default;

		// Playback
		virtual void Play(ISound* aSound) = 0;
		//virtual void Pause() = 0;
		//virtual void Resume() = 0;
		//virtual void Stop() = 0;
		//virtual PlayMode GetPlayMode() const = 0;
		//virtual bool IsValid() const = 0;
		//bool IsPlaying() const { return GetPlayMode() == PlayMode::Playing; }

		//// Mode
		//virtual void SetMode(AudioMode mode) = 0;
		//virtual AudioMode GetMode() const = 0;

		//// Mixing
		//virtual void ApplyVolume() =0 ;
		//virtual void SetPitch(float pitch) = 0;
		//virtual float GetPitch() const = 0;
		//virtual void SetPan(float pan) = 0; // -1 left 1 right
		//virtual void SetMuted(bool muted) = 0;
		//virtual bool IsMuted() const = 0;
		//virtual void FadeTo(float volume, float seconds) = 0;

		//// Time
		//virtual void SetLoopCount(int count) = 0; // -1 infinite
		//virtual void SetPosition(unsigned ms) = 0;
		//virtual unsigned GetPosition() const = 0;
		//virtual unsigned GetLength() const = 0;

		//// 3D
		//virtual void Set3DAttributes(const Math::Vector2f& position, const Math::Vector2f& velocity) = 0;
		//virtual void Set3DMinMaxDistance(float minDistance, float maxDistance) = 0;


	public:
		//__declspec(property(get = GetPitch, put = SetPitch)) float Pitch;
		//__declspec(property(get = IsMuted, put = SetMuted)) bool Muted;
		//__declspec(property(get = IsPlaying)) bool Playing;
	};
}