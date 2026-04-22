#pragma once

#include "AssetEngine/Runtime/Loader/IAssetLoader.h"
#include "AssetEngine/Data/TextureData.h"

#include <fstream>


namespace Eclipse
{
	class TextureLoader : public IAssetLoader<TextureData>
	{
	public:
		TextureData* Load(std::ifstream& in) const override;
	};
}