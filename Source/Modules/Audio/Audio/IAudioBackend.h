#pragma once

#include "AudioResult.h"
#include "IAudioChannel.h"
#include "ISound.h"

#include <Core/Standard/Core/Types.hpp>

namespace Eclipse::Audio
{
	struct SoundInfo
	{
		int   sampleRate = 0;
		int   channels = 0;
		float lengthSeconds = 0.f; 
	};

	class IAudioBackend
	{
	public:
		virtual ~IAudioBackend() = default;

	public:
		virtual IAudioChannel* CreateAudioChannel() = 0;
		virtual void* CreateSound(const void* someByteData, Size aSize, SoundInfo* outInfo) const = 0;
		virtual void GetSoundInfo(const std::filesystem::path& aPath, SoundInfo* outInfo) const = 0;

	public:
		virtual AudioError Initialize() = 0;

		virtual void Update() = 0;
		virtual void Shutdown() = 0;

		[[nodiscard]]
		virtual bool IsInitialized() const = 0;
	};
}