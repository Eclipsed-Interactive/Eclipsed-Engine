#pragma once

#include "BaseAsset.h"
#include "AssetEngine/Data/MaterialData.h"

#include "CoreEngine/GraphicsBuffers/MaterialBuffer.h"

namespace Eclipse
{
	class Material
	{
		BASE_ASSET(Material);

	public:
		Texture& GetTexture();
		const Texture& GetTexture() const;

		VertexShader& GetVertexShader();
		const VertexShader& GetVertexShader() const;

		PixelShader& GetPixelShader();
		const PixelShader& GetPixelShader() const;

		unsigned GetShaderProgramID() const;

	public:
		void BindTexture();
		void BindShader();
		void BindColor();

		void Use();
		void Create();

		MaterialBuffer myMaterialBuffer;
	};
}
