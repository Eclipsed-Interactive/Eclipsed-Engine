//#include "MetaSerializer.h"
//
//#include <iostream>
//#include <fstream>
//
//#include "AssetSystem/MetaData/Data/TextureMeta.h"
//#include "AssetSystem/MetaData/Data/AudioMeta.h"
//
//#include "Core/PathManager.h"
//
//#include "AssetSystem/SupportedAssets.h"
//
//namespace Eclipse::AssetSystem
//{
//	bool MetaSerializer::MetaFileExists(const std::filesystem::path& path)
//	{
//		return std::filesystem::exists(GetMetaPath(path));
//	}
//
//	AssetMeta MetaSerializer::LoadOrCreateMeta(const std::filesystem::path& assetPath)
//	{
//		AssetMeta meta;
//
//		if (MetaFileExists(assetPath)) meta = LoadMeta(assetPath);
//		else meta = CreateMeta(assetPath);
//
//		return meta;
//	}
//
//	AssetMeta MetaSerializer::GetMetaType(const std::filesystem::path& assetPath)
//	{
//		AssetMeta meta;
//		meta.type = GetAssetTypeFromExtension(assetPath.extension().string());
//		meta.sourcePath = assetPath;
//
//		switch (meta.type)
//		{
//		case AssetType::Texture:
//		{
//			meta.AddMetaComponent<TextureMeta>();
//			break;
//		}
//
//		case AssetType::AudioClip:
//		{
//			meta.AddMetaComponent<AudioMeta>();
//			break;
//		}
//
//		case AssetType::Unknown:
//		case AssetType::Material:
//		case AssetType::VertexShader:
//		case AssetType::PixelShader:
//		case AssetType::Font:
//		case AssetType::Prefab:
//		default:
//			break;
//		}
//
//		return meta;
//	}
//
//	AssetMeta MetaSerializer::CreateMeta(const std::filesystem::path& assetPath)
//	{
//		AssetMeta meta = GetMetaType(assetPath);
//		meta.guid.Generate();
//		meta.sourcePath = assetPath;
//		meta.type = GetAssetTypeFromExtension(assetPath.extension().string());
//
//		WriteMetaToFile(assetPath, meta);
//
//		return meta;
//	}
//	
//	AssetMeta MetaSerializer::LoadMeta(const std::filesystem::path& assetPath)
//	{
//		std::filesystem::path metaPath = GetMetaPath(assetPath);
//		std::ifstream _if(metaPath);
//
//		AssetMeta meta = GetMetaType(assetPath);
//		meta.sourcePath = assetPath;
//
//		meta.LoadFromStream(_if);
//
//		return meta;
//	}
//
//	void MetaSerializer::WriteMetaToFile(const std::filesystem::path& assetPath, const AssetMeta& assetMeta)
//	{
//		std::filesystem::path metaPath = GetMetaPath(assetPath);
//		std::ofstream of(metaPath);
//
//		assetMeta.WriteToStream(of);
//	}
//
//	std::filesystem::path MetaSerializer::GetMetaPath(const std::filesystem::path& assetPath)
//	{
//		if (assetPath.extension().string() == ".meta")
//		{
//			return assetPath;
//		}
//		else
//		{
//			return assetPath.generic_string() + ".meta";
//		}
//	}
//}