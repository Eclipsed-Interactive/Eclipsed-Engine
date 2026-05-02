#pragma once

#include <filesystem>

#include "AssetEngine/Pipeline/MetaData/AssetMeta.h"

#include "TypeCompilers/EditorTextureCompiler.h"
//#include "TypeImporters/EditorPrefabImporter.h"
//#include "TypeImporters/EditorMaterialImporter.h"
//#include "TypeImporters/EditorAudioImporter.h"
//#include "TypeImporters/EditorFontImporter.h"
//#include "TypeImporters/EditorShaderImporter.h"

namespace Eclipse::Assets
{
	class EditorAssetCompiler
	{
	public:
		static void Compile(const AssetMeta& meta);
		static void CompileAll();

	private:
		//static inline AudioImporter audioImporter;
		//static inline PrefabImporter prefabImporter;
		//static inline FontImporter fontImporter;
		static inline TextureCompiler textureCompiler;
		//static inline MaterialImporter materialImporter;
		//static inline VertexShaderImporter vertexShaderImporter;
		//static inline PixelShaderImporter pixelShaderImporter;
	};
}