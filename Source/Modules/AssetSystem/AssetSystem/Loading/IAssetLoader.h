#pragma once 

#include <Core/GUID.h>
#include <span>
#include <AssetSystem/Import/ArtifactReader.h>

namespace Eclipse::AssetSystem
{
    struct AssetData;
	class IAssetLoader
	{
	public:
        virtual ~IAssetLoader() = default;

		virtual unsigned int GetMagic() const = 0;
		virtual AssetData* Create() const = 0;
		virtual bool Load(ArtifactReader& aReader, AssetData* aData) const = 0;
		virtual bool Unload(AssetData* aData) const = 0;
	};
}