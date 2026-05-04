#include "MaterialAsset.h"

namespace Eclipse::Assets
{
	Texture Material::GetTexture() const
	{
		return dataPtr->texture;
	}

	PixelShader Material::GetPixelShader() const
	{
		return dataPtr->pixelShader;
	}

	VertexShader Material::GetVertexShader() const
	{
		return dataPtr->vertexShader;
	}
}