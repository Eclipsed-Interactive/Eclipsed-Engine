#include "AssetManager.h"

#include "CoreEngine/MainSingleton.h"
#include "AssetEngine/Core/EditorAssetDatabase.h"

#include "AssetTypes/AudioAssetType.h"
#include "AssetTypes/FontAssetType.h"
#include "AssetTypes/MaterialAssetType.h"
#include "AssetTypes/PrefabAssetType.h"
#include "AssetTypes/ShaderAssetType.h"
#include "AssetTypes/TextureAssetType.h"
#include "IO/BinaryWriter.h"

namespace Eclipse::Assets
{
	void AssetManager::ImportAssets(const std::filesystem::path& path, const std::string& key)
	{
		AssetDatabase& database = MainSingleton::GetInstance<AssetDatabase>();
		database.ProcessSource(path, key);

		for (auto& [guid, file] : database.GetSources())
		{
			AssetType assetType = GetAssetTypeFromExtension(path.extension().string());
			IAssetType* type = GetType(assetType);

			ImportedData imported = type->Import(file);
			ProcessedData processed = type->Process(imported);

			BinaryWriter writer;
			type->Serialize(writer, processed);
		}
	}

	void AssetManager::RegisterTypes()
	{
		types[AssetType::AudioClip] = new AudioAssetType;
		types[AssetType::Font] = new FontAssetType;
		types[AssetType::Material] = new MaterialAssetType;
		types[AssetType::Prefab] = new PrefabAssetType;
		types[AssetType::Shader] = new ShaderAssetType;
		types[AssetType::Texture] = new TextureAssetType;
	}

	IAssetType* AssetManager::GetType(AssetType assetType)
	{
		auto it = types.find(assetType);
		if (it == types.end()) return nullptr;

		return it->second;
	}
}