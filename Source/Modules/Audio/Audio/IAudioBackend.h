#pragma once

#include "AudioResult.h"

namespace Eclipse::Audio
{
	class IAudioBackend
	{
	public:
		virtual ~IAudioBackend() = default;

	public:
		virtual AudioError Initialize() = 0;

		virtual void Update() = 0;
		virtual void Shutdown() = 0;

		[[nodiscard]]
		virtual bool IsInitialized() const = 0;
	};
}