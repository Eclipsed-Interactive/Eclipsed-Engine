#pragma once

#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"

#include "PcmData.h"

namespace Eclipse
{
	class AudioManager
	{
	public:
		static void Init();
		static void Update();

		static void PlayAudio(FMOD::Sound* aSound, FMOD::Channel** aChannel);

	private:
		static FMOD::System* mySystem;
		static FMOD::Studio::System* myStudioSystem;
	};
}