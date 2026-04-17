#pragma once

#include "AssetEngine/Runtime/Loader/IAssetLoader.h"
#include "AssetEngine/Data/ShaderData.h"

#include <fstream>
#include "AssetEngine/SupportedTypes.h"


namespace Eclipse
{
	template<typename T, AssetType Type>
	class ShaderLoader : public IAssetLoader<ShaderData>
	{
	public:
		T* Load(std::ifstream& in) const override;
	};

	class VertexShaderLoader : public ShaderLoader<VertexShaderData, AssetType::VertexShader> {};
	class PixelShaderLoader : public ShaderLoader<PixelShaderData, AssetType::PixelShader> {};
}