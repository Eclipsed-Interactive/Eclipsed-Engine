#pragma once

#include <AssetSystem/Import/IAssetImporter.h>

namespace Eclipse::AssetSystem
{
	class TextureImporter : public IAssetImporter
	{
		bool Import(ImportContext& ctx) const override;

		constexpr unsigned int GetMagic() const { return 'ETEX'; }
	};
}