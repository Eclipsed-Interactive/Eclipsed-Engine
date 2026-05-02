#pragma once

#include "AssetEngine/Pipeline/Discovery/AssetScanner.h"
#include "MetaData/AssetMeta.h"
#include <unordered_map>
#include <vector>

namespace Eclipse::Assets
{
	class AssetDatabase 
	{
	public:
		void ProcessSource(const std::filesystem::path& root, const std::string& customKey);

	public:
		const AssetMeta& GetAssetMetaFromPath(const std::string& path) const;
		const AssetMeta& GetAssetMetaFromGuid(const GUID& guid) const;
		const std::unordered_map<GUID, AssetMeta, GUIDHash>& GetGuidToAssetMetaMap() const;

	private:
		void ImportAsset(const std::string& customKey, const AssetCandidate& candidate);

	private:
		std::unordered_map<GUID, AssetMeta, GUIDHash> guidToAsset;
		std::unordered_map<std::filesystem::path, GUID> pathToGuid;
		std::unordered_map<std::string, std::vector<GUID>> sourceToAssets;
		std::unordered_map<AssetType, std::vector<GUID>> typeToAssets;
	};
}