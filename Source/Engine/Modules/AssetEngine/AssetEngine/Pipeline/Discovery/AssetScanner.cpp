#include "AssetScanner.h"

#include "AssetEngine/Pipeline/Discovery/FileSystem.h"
#include "AssetEngine/SupportedAssets.h"

namespace Eclipse::Assets
{
	std::vector<AssetCandidate> AssetScanner::Scan(const std::filesystem::path& root)
	{
		const std::vector<FileEntry> files = FileSystem::ScanDirectory(root);

		std::vector<AssetCandidate> result;
		for (const FileEntry& file : files)
		{
			if (IsValidAssetFile(file.fullPath))
			{
				AssetCandidate candidate;
				candidate.fullPath = file.fullPath;
				candidate.relativePath = file.relativePath;
				candidate.rootPath = root;

				result.push_back(candidate);
			}
		}

		return result;
	}
	bool AssetScanner::IsValidAssetFile(const std::filesystem::path& path)
	{
		std::string extension = path.extension().string();
		return extensionToAssetType.find(extension) != extensionToAssetType.end();
	}
}