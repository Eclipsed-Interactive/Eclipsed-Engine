#pragma once

#include "AssetEngine/Runtime/Loader/IAssetLoader.h"
#include "AssetEngine/Data/PrefabData.h"

#include <fstream>


namespace Eclipse
{
	class PrefabLoader : public IAssetLoader<PrefabData>
	{
	public:
		PrefabData* Load(std::ifstream& in) const override;
	};
}