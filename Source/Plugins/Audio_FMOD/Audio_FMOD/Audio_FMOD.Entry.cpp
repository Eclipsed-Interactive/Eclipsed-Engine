#include "Audio_FMOD.Entry.h"

#include "FMOD_AudioBackend.h"


using namespace Eclipse::Audio;

AUDIO_FMOD_API IAudioBackend* CreateAudioBackend()
{
	return new FMOD_Backend;
}

AUDIO_FMOD_API void DestroyAudioBackend(IAudioBackend* audioBackend)
{
	return void();
}
