#include "MaterialAsset.h"

namespace Eclipse::Assets
{
	Texture Material::GetTexture() const
	{
		return dataPtr->texture;
	}

	Shader Material::GetShader() const
	{
		return dataPtr->shader;
	}
}