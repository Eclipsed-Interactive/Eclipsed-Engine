#pragma once

#include "AssetEngine/Runtime/Loader/IAssetLoader.h"
#include "AssetEngine/Data/AudioData.h"
#include <fstream>

namespace Eclipse
{
	class AudioLoader : public IAssetLoader<AudioClipData>
	{
	public:
		AudioClipData* Load(std::ifstream& in) const override;
	};
}