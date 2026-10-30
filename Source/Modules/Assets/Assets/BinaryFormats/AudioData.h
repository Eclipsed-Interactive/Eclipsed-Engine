#pragma once

#include "Assets/BinaryFormats/AssetData.h"
#include "Assets/Core/ImportedData.h"

#include <Audio/ISound.h>

namespace FMOD {
	class Sound;
}

namespace Eclipse::Assets
{
	struct AudioData : public AssetData {
		~AudioData()
		{
			// Runs before members are destroyed, so Data is still valid while FMOD releases the sound.
			delete sound;
		}

		Audio::ISound* sound = nullptr;

		AudioStorage Storage = AudioStorage::PCM;
		AudioFormat Format = AudioFormat::PCM16;

		unsigned int SampleRate = 0;
		unsigned int FrameCount = 0;
		int Channels = 0;

		bool Streaming = false;

		std::vector<unsigned char> Data;
	};
}