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
	class IAudioChannel;
	class ISound;
	class FMOD_Backend :  public IAudioBackend
	{
	public:
		AudioError Initialize() override;

		void Update() override;
		void Shutdown() override;

		[[nodiscard]]
		bool IsInitialized() const override;

	public:
		IAudioChannel* CreateAudioChannel() override;
		void* CreateSound(const void* someByteData, Size aSize, SoundInfo* outInfo) const override;
		void GetSoundInfo(const std::filesystem::path& aPath, SoundInfo* outInfo) const override;
	
	private:
		void ReleaseStudioSystem();
		void ResetBackend();


	public:
		static FMOD::System* system ;
		static FMOD::Studio::System* studioSystem ;

	private:
		bool isInitialized = false;
	};
}