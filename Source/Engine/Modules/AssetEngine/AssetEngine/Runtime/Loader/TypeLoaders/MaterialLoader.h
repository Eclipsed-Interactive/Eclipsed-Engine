#pragma once

#include "AssetEngine/Runtime/Loader/IAssetLoader.h"
#include "AssetEngine/Data/MaterialData.h"
#include <fstream>

namespace Eclipse
{
	class MaterialLoader : public IAssetLoader<MaterialData>
	{
	public:
		MaterialData* Load(std::ifstream& in) const override;
	};
}