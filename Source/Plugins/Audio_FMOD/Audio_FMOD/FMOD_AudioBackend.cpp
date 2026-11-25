#include "FMOD_AudioBackend.h"

#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"
#include <fmod/fmod_errors.h>

#include "FMOD_AudioChannel.h"
#include "FMOD_Sound.h"

#include <Core/MainSingleton.h>

namespace Eclipse::Audio
{
	FMOD::System* FMOD_Backend::system = nullptr;
	FMOD::Studio::System* FMOD_Backend::studioSystem = nullptr;

	AudioError TranslateFMODError(const FMOD_RESULT result, const AudioError fallback = AudioError::BackendOperationFailed)
	{
		switch (result)
		{
		case FMOD_OK:
			return AudioError::None;

		case FMOD_ERR_INVALID_PARAM:
			return AudioError::InvalidArgument;

		case FMOD_ERR_INVALID_HANDLE:
			return AudioError::InvalidState;

		case FMOD_ERR_UNINITIALIZED:
			return AudioError::NotInitialized;

		case FMOD_ERR_INITIALIZED:
			return AudioError::AlreadyInitialized;

		case FMOD_ERR_MEMORY:
			return AudioError::OutOfMemory;

		case FMOD_ERR_VERSION:
			return AudioError::BackendVersionMismatch;

		case FMOD_ERR_OUTPUT_NODRIVERS:
			return AudioError::DeviceUnavailable;

		//case FMOD_ERR_OUTPUT_DEVICENOTFOUND:
		//	return AudioError::DeviceUnavailable;

		case FMOD_ERR_OUTPUT_FORMAT:
			return AudioError::UnsupportedFormat;

		case FMOD_ERR_FILE_NOTFOUND:
			return AudioError::ResourceNotFound;

		case FMOD_ERR_FORMAT:
			return AudioError::UnsupportedFormat;

		default:
			return fallback;
		}
	}

	AudioError FMOD_Backend::Initialize()
	{
		if (isInitialized)
			return AudioError::AlreadyInitialized;

		FMOD_RESULT result = FMOD::Studio::System::create(&studioSystem);

		if (result != FMOD_OK)
		{
			studioSystem = nullptr;
			return TranslateFMODError(result);
		}

		result = studioSystem->getCoreSystem(&system);

		if (result != FMOD_OK)
		{
			studioSystem->release();
			studioSystem = nullptr;
			system = nullptr;
			return TranslateFMODError(result);
		}

		result = studioSystem->initialize(
			512,
			FMOD_STUDIO_INIT_NORMAL,
			FMOD_INIT_NORMAL,
			nullptr
		);

		if (result != FMOD_OK)
		{
			studioSystem->release();
			studioSystem = nullptr;
			system = nullptr;
			return TranslateFMODError(result);
		}

		MainSingleton::AddInstance<FMOD::System*>(system);

		isInitialized = true;
		return AudioError::None;
	}

	void FMOD_Backend::Update()
	{
		if (!isInitialized) return;

		studioSystem->update();
	}

	void FMOD_Backend::Shutdown()
	{
		ReleaseStudioSystem();
	}

	IAudioChannel* FMOD_Backend::CreateAudioChannel()
	{
		return new FMOD_AudioChannel;
	}

	void* FMOD_Backend::CreateSound(const void* someByteData, Size aSize, SoundInfo* outInfo) const
	{
		if (!system || !someByteData || aSize == 0) return nullptr;

		FMOD_CREATESOUNDEXINFO exinfo{};
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = static_cast<unsigned int>(aSize);

		FMOD::Sound* sound = nullptr;

		if (system->createSound(static_cast<const char*>(someByteData), FMOD_OPENMEMORY | FMOD_CREATESAMPLE, &exinfo, &sound) != FMOD_OK)
			return nullptr;

		if (outInfo)
		{
			float freq = 0.f; int priority = 0;
			sound->getDefaults(&freq, &priority);
			outInfo->sampleRate = static_cast<int>(freq);

			FMOD_SOUND_TYPE type; FMOD_SOUND_FORMAT format; int bits = 0;
			sound->getFormat(&type, &format, &outInfo->channels, &bits);

			unsigned int lengthMs = 0;
			sound->getLength(&lengthMs, FMOD_TIMEUNIT_MS);
			outInfo->lengthSeconds = lengthMs / 1000.f;
		}

		return sound;
	}

	void FMOD_Backend::GetSoundInfo(const std::filesystem::path& aPath, SoundInfo* outInfo) const
	{
		if (!system || !std::filesystem::exists(aPath) ) return;

		FMOD::Sound* sound = nullptr;

		FMOD_RESULT r = system->createSound(aPath.generic_string().c_str(), FMOD_OPENONLY, nullptr, &sound);
		if (r != FMOD_OK)
		{
			printf("FMOD createSound failed (%d): %s  path=%s\n", r, FMOD_ErrorString(r), aPath.generic_u8string().c_str());
			return;
		}

		if (outInfo)
		{
			float freq = 0.f; int priority = 0;
			sound->getDefaults(&freq, &priority);
			outInfo->sampleRate = static_cast<int>(freq);

			FMOD_SOUND_TYPE type; FMOD_SOUND_FORMAT format; int bits = 0;
			sound->getFormat(&type, &format, &outInfo->channels, &bits);

			unsigned int lengthMs = 0;
			sound->getLength(&lengthMs, FMOD_TIMEUNIT_MS);
			outInfo->lengthSeconds = lengthMs / 1000.f;
		}

		sound->release();
	}

	bool FMOD_Backend::IsInitialized() const
	{
		return isInitialized;
	}

	void FMOD_Backend::ReleaseStudioSystem()
	{
		if (!isInitialized) return;

		studioSystem->release();
		ResetBackend();
	}
	
	void FMOD_Backend::ResetBackend()
	{
		studioSystem = nullptr;
		system = nullptr;
		isInitialized = false;
	}
}


