#pragma once

#include "Audio.Core.hpp"

#include "AudioResult.h"
#include <filesystem>

#include "IAudioBackend.h"

namespace Eclipse::Audio
{
	class AUDIO_API AudioManager
	{
	public:
		static AudioError LoadBackend(const std::filesystem::path& path);

	public:
		static IAudioBackend* GetBackend();

	private:
		using CreateBackendFn = IAudioBackend * (*)();
		using DestroyBackendFn = void (*)(IAudioBackend*);

		static void* ActiveBackendDLL;
		static DestroyBackendFn DestroyBackend;

		static IAudioBackend* ActiveBackend;
	};
}