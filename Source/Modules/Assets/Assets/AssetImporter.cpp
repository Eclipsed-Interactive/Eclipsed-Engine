#include "Assets/AssetImporter.h"

#include "Core/MainSingleton.h"

#include "Core/AssetDatabase.h"
#include "Core/ImportedData.h"
#include "AssetTypeRegistry.h"
#include "Metadata/AssetMeta.h"
#include "AssetTypes/IAssetType.h"
#include "IO/BinaryWriter.h"
#include <Core/Profiling/ProfilerMacros.hpp>

namespace Eclipse::Assets
{
	void AssetImporter::ImportAssets(const std::filesystem::path& root, const std::string& key)
	{
		PROFILE_SCOPED;
		if (!MainSingleton::Exists<AssetDatabase>())
		{
			MainSingleton::RegisterInstance<AssetDatabase>();
			AssetTypeRegistry::RegisterTypes();
		}

		AssetDatabase& database = MainSingleton::GetInstance<AssetDatabase>();

		database.ProcessSource(root, key);

		for (auto& [guid, file] : database.GetSources())
		{
			ImportFile(file);
		}
	}

	void AssetImporter::ImportFile(const AssetMeta& meta)
	{
		PROFILE_SCOPED;
		if (meta.fullPath.empty()) return;

		AssetType assetType = GetAssetTypeFromExtension(meta.fullPath.extension().string());
		IAssetType* type = AssetTypeRegistry::GetType(assetType);
		if (type == nullptr) return; // Type is not supported.

		if (std::filesystem::exists(meta.exportedPath) && std::filesystem::last_write_time(meta.fullPath) < std::filesystem::last_write_time(meta.exportedPath)) return;

		ImportedData imported = type->Import(meta);
		BinaryWriter writer(meta.guid);

		if (type->NeedsProcessing())
		{
			ProcessedData processed = type->Process(imported);
			type->Serialize(writer, processed);
		}
		else
		{
			type->Serialize(writer, imported);
		}
	}

	void AssetImporter::ImportBundle()
	{
		if (!MainSingleton::Exists<AssetDatabase>())
		{
			MainSingleton::RegisterInstance<AssetDatabase>();
			AssetTypeRegistry::RegisterTypes();
		}

		AssetDatabase& database = MainSingleton::GetInstance<AssetDatabase>();
		database.ProcessBundle("assets.bundle");
	}
}