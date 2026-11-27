#pragma once 

#include <Core/GUID.h>
#include <span>
#include "AssetSystem/Loading/IAssetLoader.h"
#include <AssetSystem/Assets/Data/ShaderData.hpp>

namespace Eclipse::AssetSystem
{
	struct AssetData;
	class ShaderLoader : public IAssetLoader
	{
	public:
		virtual ~ShaderLoader() = default;

		unsigned int GetMagic() const override { return 'ESHA'; }
		AssetData* Create() const override { return new ShaderData; }
		bool Load(ArtifactReader& aReader, AssetData* aData) const override;
		bool Unload(AssetData* aData) const override;

	};
}