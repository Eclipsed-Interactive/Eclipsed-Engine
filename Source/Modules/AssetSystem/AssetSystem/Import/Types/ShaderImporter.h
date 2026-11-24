#pragma once

// Vertex Shader
// PixelShade
// Compute Shader
// Geometry Shader

#include <AssetSystem/Import/IAssetImporter.h>

namespace Eclipse::AssetSystem
{
	class ShaderImporter : public IAssetImporter
	{
		bool Import(ImportContext& ctx) const override;

		constexpr unsigned int GetMagic() const { return 'ESHA'; }
	};
}