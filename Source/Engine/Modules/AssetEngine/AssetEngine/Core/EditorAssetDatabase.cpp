#include "EditorAssetDatabase.h"

#include "AssetEngine/Discovery/AssetScanner.h"

namespace Eclipse::Assets
{
	void AssetDatabase::ProcessSource(const std::filesystem::path& path, const std::string& key)
	{
		std::vector<AssetCandidate> candidates = AssetScanner::Scan(path);

		for (const AssetCandidate& candidate : candidates)
		{
			AssetMeta file;
			file.guid.Generate();
			file.fullPath = candidate.fullPath;

			guidToAsset[file.guid] = file;
			pathToGuid[candidate.fullPath] = file.guid;
			const AssetType type = GetAssetTypeFromExtension(candidate.fullPath.extension().string());
			typeToAssets[type].push_back(file.guid);
			sourceToAssets[key].push_back(file.guid);
		}
	}

	const AssetMeta& AssetDatabase::GetProcessedFile(const std::filesystem::path& path) const
	{
		const auto pathIt = pathToGuid.find(path);
		if (pathIt == pathToGuid.end()) throw std::runtime_error("The path cant be found in the map.");

		const auto guidIt = guidToAsset.find(pathToGuid.at(path));
		if (guidIt == guidToAsset.end()) throw std::runtime_error("The guid cant be found in the map.");

		return guidIt->second;
	}

	const AssetMeta& AssetDatabase::GetProcessedFile(const GUID& guid) const
	{
		const auto it = guidToAsset.find(guid);
		if (it == guidToAsset.end()) throw std::runtime_error("The guid cant be found in the map.");

		return it->second;
	}

	const std::vector<GUID>& AssetDatabase::GetGUIDFromSource(const std::string& source)
	{
		const auto it = sourceToAssets.find(source);
		if (it == sourceToAssets.end()) throw std::runtime_error("The guid cant be found in the map.");

		return it->second;
	}

	const std::unordered_map<GUID, AssetMeta, GUIDHash>& AssetDatabase::GetSources() const
	{
		return guidToAsset;
	}
}