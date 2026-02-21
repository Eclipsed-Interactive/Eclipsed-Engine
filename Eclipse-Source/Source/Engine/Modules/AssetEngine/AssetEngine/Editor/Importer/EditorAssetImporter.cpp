#include "EditorAssetImporter.h"

#include "AssetEngine/SupportedTypes.h"
#include "AssetEngine/Editor/MetaFile/AssetMetaManager.h"
#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"

namespace Eclipse
{
	void EditorAssetImporter::Import(const std::filesystem::path& aPath)
	{
		AssetType type = GetAssetTypeFromExtension(aPath.extension().generic_string());
		switch (type)
		{
		case AssetType::Texture: textureImporter.Import(aPath); break;

		case AssetType::Material: materialImporter.Import(aPath); break;

		case AssetType::Prefab: prefabImporter.Import(aPath); break;

		case AssetType::VertexShader: vertexShaderImporter.Import(aPath); break;
		case AssetType::PixelShader: pixelShaderImporter.Import(aPath); break;

		case AssetType::AudioClip: audioImporter.Import(aPath); break;
		}
	}

	void EditorAssetImporter::ImportAll(const std::filesystem::path& aRootPath)
	{
		if (!std::filesystem::exists(aRootPath)) return;

		for (auto& entry : std::filesystem::recursive_directory_iterator(aRootPath))
		{
			std::filesystem::path path = entry.path();

			if (std::filesystem::is_directory(entry)) continue;
			else if (entry.path().extension() == ".meta") continue;

			Import(entry);
		}
	}
}