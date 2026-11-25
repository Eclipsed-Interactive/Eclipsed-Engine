#pragma once

#include "AssetData.h"

#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	struct ASSETSYSTEM_API AudioData : AssetData
	{
		int   sampleRate = 0;
		int   channels = 0;
		float lengthSeconds = 0.f;

		void* sound = nullptr;
	};
}