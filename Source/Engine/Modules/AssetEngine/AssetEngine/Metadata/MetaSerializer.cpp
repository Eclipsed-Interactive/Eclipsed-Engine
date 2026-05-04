#include "MetaSerializer.h"

#include <iostream>
#include <fstream>

#include "Data/TextureMeta.h"

namespace Eclipse::Assets
{
	bool MetaSerializer::MetaFileExists(const std::filesystem::path& path)
	{
		return std::filesystem::exists(GetMetaPath(path));
	}

	AssetMeta MetaSerializer::LoadOrCreateMeta(const std::filesystem::path& assetPath)
	{
		if (MetaFileExists(assetPath)) return LoadMeta(assetPath);
		else return CreateMeta(assetPath);
	}

	AssetMeta MetaSerializer::GetMetaType(const std::filesystem::path& assetPath)
	{
		AssetMeta meta;
		meta.type = GetAssetTypeFromExtension(assetPath.extension().string());
		meta.fullPath = assetPath;

		switch (meta.type)
		{
		case AssetType::Texture:
		{
			meta.AddMetaComponent<TextureMeta>();
			break;
		}

		case AssetType::Unknown:
		case AssetType::Material:
		case AssetType::Shader:
		case AssetType::AudioClip:
		case AssetType::Font:
		case AssetType::Prefab:
		default:
			break;
		}

		return meta;
	}

	AssetMeta MetaSerializer::CreateMeta(const std::filesystem::path& assetPath)
	{
		AssetMeta meta = GetMetaType(assetPath);
		meta.guid.Generate();

		WriteMetaToFile(assetPath, meta);

		return meta;
	}
	
	AssetMeta MetaSerializer::LoadMeta(const std::filesystem::path& assetPath)
	{
		std::filesystem::path metaPath = GetMetaPath(assetPath);
		std::ifstream _if(metaPath);

		AssetMeta meta = GetMetaType(assetPath);
		meta.LoadFromStream(_if);

		return meta;
	}

	void MetaSerializer::WriteMetaToFile(const std::filesystem::path& assetPath, const AssetMeta& assetMeta)
	{
		std::filesystem::path metaPath = GetMetaPath(assetPath);
		std::ofstream of(metaPath);

		assetMeta.WriteToStream(of);
	}

	std::filesystem::path MetaSerializer::GetMetaPath(const std::filesystem::path& assetPath)
	{
		return assetPath.generic_string() + ".meta";
	}
}