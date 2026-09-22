#include "AudioPluginLoader.h"

#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eclipse::Audio
{
	void* AudioManager::ActiveBackendDLL;
	AudioManager::DestroyBackendFn AudioManager::DestroyBackend;

	IAudioBackend* AudioManager::ActiveBackend;

	AudioError AudioManager::LoadBackend(const std::filesystem::path& path)
	{
		if (ActiveBackendDLL != nullptr)
		{
			return AudioError::AlreadyInitialized;
		}

		if (path.empty())
		{
			return AudioError::InvalidArgument;
		}

		std::error_code filesystemError;
		const bool exists = std::filesystem::exists(path, filesystemError);

		if (filesystemError || !exists)
		{
			return AudioError::BackendUnavailable;
		}

		HMODULE backend = LoadLibraryW(path.c_str());
		if (backend == nullptr)
		{
			return AudioError::ResourceLoadFailed;
		}

		const CreateBackendFn createBackend = reinterpret_cast<CreateBackendFn>(GetProcAddress(backend, "CreateAudioBackend"));
		const DestroyBackendFn destroyBackend = reinterpret_cast<DestroyBackendFn>(GetProcAddress(backend, "DestroyAudioBackend"));

		if (createBackend == nullptr || destroyBackend == nullptr)
		{
			FreeLibrary(backend);
			return AudioError::BackendVersionMismatch;
		}

		IAudioBackend* instance = createBackend();
		if (instance == nullptr)
		{
			FreeLibrary(backend);
			return AudioError::BackendInitializationFailed;
		}

		ActiveBackendDLL = backend;
		ActiveBackend = instance;
		DestroyBackend = destroyBackend;

		return AudioError::Succeded;
	}

	IAudioBackend* AudioManager::GetBackend()
	{
		return ActiveBackend;
	}
}