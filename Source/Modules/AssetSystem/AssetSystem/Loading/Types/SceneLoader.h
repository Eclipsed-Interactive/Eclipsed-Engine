#pragma once 

#include <Core/GUID.h>
#include <span>
#include "AssetSystem/Loading/IAssetLoader.h"
#include <AssetSystem/Assets/Data/SceneData.hpp>

namespace Eclipse::AssetSystem
{
	struct AssetData;
	class SceneLoader : public IAssetLoader
	{
	public:
		virtual ~SceneLoader() = default;

		unsigned int GetMagic() const override { return 'ESCE'; }
		AssetData* Create() const override { return new SceneData; }
		bool Load(ArtifactReader& aReader, AssetData* aData) const override;
		bool Unload(AssetData* aData) const override;
	};
}