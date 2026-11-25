#pragma once 

#include <Core/GUID.h>
#include <span>
#include "AssetSystem/Loading/IAssetLoader.h"
#include <AssetSystem/Assets/Data/AudioData.hpp>

namespace Eclipse::AssetSystem
{
	struct AssetData;
	class AudioLoader : public IAssetLoader
	{
	public:
		virtual ~AudioLoader() = default;

		unsigned int GetMagic() const override { return 'EAUD'; }
		AssetData* Create() const override { return new AudioData; }
		bool Load(ArtifactReader& aReader, AssetData* aData) const override;
	};
}