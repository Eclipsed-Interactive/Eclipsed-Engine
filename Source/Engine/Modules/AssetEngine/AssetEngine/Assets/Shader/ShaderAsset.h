#pragma once

#include "AssetEngine/Assets/Asset.h"

#include "AssetEngine/BinaryFormats/ShaderData.h"

namespace Eclipse::Assets
{
	struct Shader : public Asset<ShaderData> {
		// methods regarding the asset.
	};
}