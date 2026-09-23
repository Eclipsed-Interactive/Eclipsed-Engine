#pragma once

#include "Audio/AudioResult.h"
#include "Audio/IAudioBackend.h"

namespace FMOD
{
	class System;
	namespace Studio { class System; }
}

namespace Eclipse::Audio
{
	class FMOD_Backend :  public IAudioBackend
	{
	public:
		AudioError Initialize() override;

		void Update() override;
		void Shutdown() override;

		[[nodiscard]]
		bool IsInitialized() const override;

	private:
		void ReleaseStudioSystem();
		void ResetBackend();

	private:
		FMOD::System* system = nullptr;
		FMOD::Studio::System* studioSystem = nullptr;

		bool isInitialized = false;
	};
}