#pragma once 

#include <Core/GUID.h>
#include <span>
#include "AssetSystem/Loading/IAssetLoader.h"
#include <AssetSystem/Assets/Data/TextureData.hpp>

namespace Eclipse::AssetSystem
{
	struct AssetData;
	class TextureLoader : public IAssetLoader
	{
	public:
		virtual ~TextureLoader() = default;

		unsigned int GetMagic() const override { return 'ETEX'; }
		AssetData* Create() const override { return new TextureData; }
		bool Load(ArtifactReader& aReader, AssetData* aData) const override;
	};
}