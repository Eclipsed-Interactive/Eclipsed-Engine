#pragma once

#include <AssetSystem/Import/IAssetImporter.h>

namespace Eclipse::AssetSystem
{
	class SceneImporter : public IAssetImporter
	{
		bool Import(ImportContext& ctx) const override;

		constexpr unsigned int GetMagic() const { return 'ESCE'; }
	};
}