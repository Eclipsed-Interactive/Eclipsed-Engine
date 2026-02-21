#pragma once

#include <filesystem>

#include "TypeImporters/EditorPrefabImporter.h"
#include "TypeImporters/EditorTextureImporter.h"
#include "TypeImporters/EditorMaterialImporter.h"
#include "TypeImporters/EditorAudioImporter.h"
#include "TypeImporters/EditorShaderImporter.h"

namespace Eclipse
{
	class EditorAssetImporter
	{
	public:
		static void Import(const std::filesystem::path& aPath);
		static void ImportAll(const std::filesystem::path& aRootPath);

	private:
		static inline AudioImporter audioImporter;
		static inline PrefabImporter prefabImporter;
		static inline TextureImporter textureImporter;
		static inline MaterialImporter materialImporter;
		static inline VertexShaderImporter vertexShaderImporter;
		static inline PixelShaderImporter pixelShaderImporter;
	};
}