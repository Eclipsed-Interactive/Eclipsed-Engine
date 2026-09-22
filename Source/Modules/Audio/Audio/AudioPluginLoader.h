#pragma once

#include "AudioResult.h"
#include <filesystem>

#include "IAudioBackend.h"

namespace Eclipse::Audio
{
	class AudioPluginLoader
	{
	public:
		AudioError LoadBackend(const std::filesystem::path& path);
		AudioError UnloadBackend();

	private:
		void* libraryHandle = nullptr;
		IAudioBackend* backend = nullptr;
		bool isInitialized = false;
	};
}