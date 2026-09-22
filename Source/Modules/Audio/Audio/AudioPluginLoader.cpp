#include "AudioPluginLoader.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eclipse::Audio
{
	AudioError AudioPluginLoader::LoadBackend(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
			return AudioError::BackendUnavailable;

		libraryHandle = LoadLibraryW(path.c_str());

		GetProcAddress((HMODULE)libraryHandle, "CreateAudioBackend");

		return AudioError();
	}

	AudioError AudioPluginLoader::UnloadBackend()
	{
		return AudioError();
	}
}