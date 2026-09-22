#include "FMOD_AudioBackend.h"

#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"

namespace Eclipse::Audio
{
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


