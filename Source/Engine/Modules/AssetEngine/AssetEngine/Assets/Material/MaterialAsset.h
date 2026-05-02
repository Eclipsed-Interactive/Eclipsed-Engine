#pragma once

#include "AssetEngine/Assets/Asset.h"

#include "AssetEngine/BinaryFormats/MaterialData.h"


namespace Eclipse::Assets
{
	struct Material : public Asset<MaterialData> {
		Texture GetTexture() const;
		Shader GetShader() const;
	};
}