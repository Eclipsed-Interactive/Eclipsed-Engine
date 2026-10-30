#pragma once

#include <Audio/ISound.h>

namespace FMOD
{
	class Channel;
	class Sound;
}

namespace Eclipse::Audio
{
	class FMOD_Sound : public ISound
	{
	public:
		~FMOD_Sound() override;

		void* GetSound() override;
		void SetSound(void* aSound) override;

	private:
		FMOD::Sound* mySound = nullptr;
	};
}