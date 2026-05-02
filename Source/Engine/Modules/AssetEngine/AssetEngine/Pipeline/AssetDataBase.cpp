#include "AssetDataBase.h"
#include <assert.h>
#include "Metadata/MetaSerializer.h"

namespace Eclipse::Assets
{
	void AssetDatabase::ProcessSource(const std::filesystem::path& root, const std::string& customKey)
	{
		std::vector<AssetCandidate> candidates = AssetScanner::Scan(root);
		for (auto& candidate : candidates)
		{
			AssetMeta meta = MetaSerializer::LoadOrCreateMeta(candidate.fullPath);
			guidToAsset[meta.guid] = meta;
			pathToGuid[candidate.fullPath] = meta.guid;
			sourceToAssets[customKey].push_back(meta.guid);
			typeToAssets[meta.type].push_back(meta.guid);
		}
	}

	const AssetMeta& Assets::AssetDatabase::GetAssetMetaFromPath(const std::string& path) const
	{
		auto it = pathToGuid.find(path);
		if (it != pathToGuid.end()) return guidToAsset.at(it->second);

		assert("GUID dont exist.");
		throw std::runtime_error("GUID Dont exist");
	}

	const AssetMeta& Assets::AssetDatabase::GetAssetMetaFromGuid(const GUID& guid) const
	{
		auto it = guidToAsset.find(guid);
		if (it != guidToAsset.end()) return it->second;

		assert("GUID dont exist.");
		throw std::runtime_error("GUID Dont exist");
	}

	const std::unordered_map<GUID, AssetMeta, GUIDHash>& Assets::AssetDatabase::GetGuidToAssetMetaMap() const
	{
		return guidToAsset;
	}

	void AssetDatabase::ImportAsset(const std::string& customKey, const AssetCandidate& candidate)
	{
	}
}