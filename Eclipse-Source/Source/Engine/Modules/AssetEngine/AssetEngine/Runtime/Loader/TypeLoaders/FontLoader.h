#pragma once

#include "AssetEngine/Runtime/Loader/IAssetLoader.h"
#include "Data/FontData.h"

namespace Eclipse
{
	class FontLoader : public IAssetLoader<FontData>
	{
	public:
		FontData* Load(std::ifstream& in) const override;
	};
}
