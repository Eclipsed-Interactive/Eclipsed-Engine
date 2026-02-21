#pragma once

#include "fmod/fmod.h"
#include "fmod/fmod.hpp"

#include "PcmData.h"

namespace Eclipse
{
	class AudioManager
	{
	public:
		static void Init();
		static void Update();

		static void PlayAudio(FMOD::Sound* aSound, FMOD::Channel** aChannel);
		static void CreateAudioFromPath(const char* path, FMOD::Sound* aSound);
		static void CreateAudioFromMemory(const char* data, size_t size, FMOD::Sound* aSound);

	private:
		static FMOD::System* mySystem;
	};
}