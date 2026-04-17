#pragma once

#include "AssetData.h"

namespace FMOD {
	class Sound;
}

namespace Eclipse
{
	struct AudioClipData : public AssetData {
		FMOD::Sound* sound;
		char* audioData = nullptr;


		~AudioClipData() {
			delete[] audioData;
		}
	};
}