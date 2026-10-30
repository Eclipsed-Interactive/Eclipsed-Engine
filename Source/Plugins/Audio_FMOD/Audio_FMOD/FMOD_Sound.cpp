#include "FMOD_Sound.h"

#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"

#include "FMOD_AudioBackend.h"

namespace Eclipse::Audio
{
	FMOD_Sound::~FMOD_Sound()
	{
		if (mySound) mySound->release();
	}

	void* FMOD_Sound::GetSound()
	{
		return mySound;
	}

	void FMOD_Sound::SetSound(void* aSound)
	{
		mySound = static_cast<FMOD::Sound*>(aSound);
	}
}