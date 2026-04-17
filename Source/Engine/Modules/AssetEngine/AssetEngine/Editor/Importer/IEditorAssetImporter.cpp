#include "IEditorAssetImporter.h"

#include "AssetEngine/SupportedTypes.h"
#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include <fstream>

#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"
#include "AssetEngine/Editor/GUID/GuidGenerator.h"

#include "CoreEngine/PathManager.h"

using namespace std::filesystem;

namespace Eclipse
{
	bool IEditorAssetImporter::CanImport(const std::filesystem::path& aPath) const
	{
		std::string ext = aPath.extension().generic_string();

		return GetAssetTypeFromExtension(ext) != AssetType::Unknown;
	}

	void IEditorAssetImporter::Import(const std::filesystem::path& aPath)
	{
		const std::filesystem::path metafilePath = MetaFileRegistry::GetMetaFilePath(aPath);

		std::string guid = "";
		if (MetaFileRegistry::MetaFileExists(aPath))
		{
			guid = MetaFileRegistry::GetGUID(aPath);
		}
		else
		{
			AssetMetaSettings importSettings = LoadOrCreateMeta<AssetMetaSettings>(aPath);
			guid = importSettings.guid;
		}

		std::filesystem::path artifactPath = GetArtifactPath(guid);

		if (!exists(artifactPath) || last_write_time(artifactPath) < last_write_time(aPath) ||
			last_write_time(artifactPath) < last_write_time(metafilePath))
		{
			std::ofstream out(artifactPath, std::ios::binary);
			Export(guid, out, aPath);
		}
	}

	bool IEditorAssetImporter::ArtifactUpToData(const std::filesystem::path& aPath)
	{
		std::filesystem::path metafilepath = aPath;
		if (aPath.extension() != ".meta")
		{
			metafilepath = MetaFileRegistry::GetMetaFilePath(aPath);
		}

		std::ifstream in(metafilepath);
		cereal::JSONInputArchive archive(in);

		AssetMetaSettings metafile;
		archive(metafile);

		
		std::filesystem::path exportFolderPath = PathManager::GetProjectRoot() / "Local/Artifacts" / metafile.guid.substr(0, 2) / metafile.guid;

		auto time0 = std::filesystem::last_write_time(exportFolderPath).time_since_epoch().count();
		auto time1 = std::filesystem::last_write_time(metafilepath).time_since_epoch().count();

		return time0 == time1;
	}

	std::filesystem::path IEditorAssetImporter::GetArtifactPath(const std::string& aGuid)
	{
		std::filesystem::path folder = PathManager::GetArtifactsPath() / aGuid.substr(0, 2);
		std::filesystem::create_directories(folder);

		return folder / aGuid;
	}
}