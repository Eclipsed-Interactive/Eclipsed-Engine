#pragma once 

#include <Core/GUID.h>
#include <span>
#include "AssetSystem/Loading/IAssetLoader.h"
#include <AssetSystem/Assets/Data/MaterialData.hpp>

namespace Eclipse::AssetSystem
{
	struct AssetData;
	class MaterialLoader : public IAssetLoader
	{
	public:
		virtual ~MaterialLoader() = default;

		unsigned int GetMagic() const override { return 'EMAT'; }
		AssetData* Create() const override { return new MaterialData; }
		bool Load(ArtifactReader& aReader, AssetData* aData) const override;
		bool Unload(AssetData* aData) const override;
	};
}