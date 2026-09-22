#pragma once

#ifdef Audio_FMOD_EXPORTS
#define AUDIO_FMOD_API __declspec(dllexport)
#elif Audio_FMOD_IMPORTS
#define AUDIO_FMOD_API __declspec(dllimport)
#else
#define  AUDIO_FMOD_API
#endif