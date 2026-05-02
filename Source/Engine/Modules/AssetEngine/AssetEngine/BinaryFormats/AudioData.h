#pragma once

#include "AssetEngine/BinaryFormats/AssetData.h"

namespace FMOD {
	class Sound;
}

namespace Eclipse::Assets
{
	struct AudioData : public AssetData {
		FMOD::Sound* sound;
		char* audioData = nullptr;


		~AudioData() {
			delete[] audioData;
		}
	};
}