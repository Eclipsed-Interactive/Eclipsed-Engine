#pragma once

#include "Audio_FMOD.Core.hpp"
#include "Audio/IAudioBackend.h"

extern "C"
{
    AUDIO_FMOD_API Eclipse::Audio::IAudioBackend* CreateAudioBackend();
    AUDIO_FMOD_API void DestroyAudioBackend(Eclipse::Audio::IAudioBackend* audioBackend);
}