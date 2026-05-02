#include "EditorAssetCompiler.h"

#include "AssetEngine/Pipeline/AssetDataBase.h"
#include "CoreEngine/MainSingleton.h"

namespace Eclipse::Assets
{
	void EditorAssetCompiler::Compile(const AssetMeta& meta)
	{
		std::filesystem::path artifactPath = meta.GetArtifactPath();
		
		std::filesystem::create_directories(artifactPath.parent_path());
		std::ofstream out(artifactPath);
		out.close();

		switch (meta.type)
		{
		case AssetType::Texture: textureCompiler.Compile(meta); break;

		//case AssetType::Material: materialImporter.Import(aPath); break;

		//case AssetType::Prefab: prefabImporter.Import(aPath); break;

		//case AssetType::Font: fontImporter.Import(aPath); break;

		//case AssetType::VertexShader: vertexShaderImporter.Import(aPath); break;
		//case AssetType::PixelShader: pixelShaderImporter.Import(aPath); break;

		//case AssetType::AudioClip: audioImporter.Import(aPath); break;
		}
	}

	void EditorAssetCompiler::CompileAll()
	{
		for (auto& [guid, meta] : MainSingleton::GetInstance<AssetDatabase>().GetGuidToAssetMetaMap())
		{
			Compile(meta);
		}
	}
}