#pragma once

#include "AudioResult.h"
#include "IAudioChannel.h"
#include "ISound.h"

namespace Eclipse::Audio
{
	class IAudioBackend
	{
	public:
		virtual ~IAudioBackend() = default;

	public:
		virtual IAudioChannel* CreateAudioChannel() = 0;
		virtual ISound* CreateSound() = 0;

	public:
		virtual AudioError Initialize() = 0;

		virtual void Update() = 0;
		virtual void Shutdown() = 0;

		[[nodiscard]]
		virtual bool IsInitialized() const = 0;
	};
}