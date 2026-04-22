#pragma once

#include "AssetEngine/Editor/Importer/IEditorAssetImporter.h"
#include "AssetEngine/SupportedTypes.h"

namespace Eclipse
{
	template<AssetType Type>
	class ShaderImporter : public IEditorAssetImporter
	{
	public:
		void Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath) override;
	};

	class PixelShaderImporter : public ShaderImporter<AssetType::PixelShader> {};
	class VertexShaderImporter : public ShaderImporter<AssetType::VertexShader> {};
}