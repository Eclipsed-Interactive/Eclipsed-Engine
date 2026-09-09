#pragma once

#include "Assets/Assets/Asset.h"

#include "Assets/BinaryFormats/MaterialData.h"
#include "Assets.Core.hpp"
#include "Core/GraphicsBuffers/MaterialBuffer.h"


namespace Eclipse::Assets
{
	struct ASSET_API Material : public Asset<MaterialData> {
		ASSET_IMPL(Material, MaterialData)

	public:
		Texture& GetTexture() const;
		PixelShader& GetPixelShader() const;
		VertexShader& GetVertexShader() const;

	public:
		unsigned GetProgramID() const;

		MaterialBuffer& GetBuffer();

	private:
		MaterialBuffer materialBuffer;
	};
}