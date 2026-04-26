#include "IEditorAssetImporter.h"

#include "AssetEngine/SupportedTypes.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include <fstream>
#include <string>

#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"
#include "AssetEngine/Editor/GUID/GuidGenerator.h"

#include "CoreEngine/PathManager.h"
#include "AssetEngine/AssetDatabase.h"

#include "CoreEngine/MainSingleton.h"

using namespace std::filesystem;

namespace Eclipse
{
	bool IEditorAssetImporter::CanImport(const std::filesystem::path& aPath) const
	{
		return false;
		std::string ext = aPath.extension().generic_string();

		return GetAssetTypeFromExtension(ext) != AssetType::Unknown;
	}

	void IEditorAssetImporter::Import(const std::filesystem::path& aPath)
	{
		Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
		const std::filesystem::path metafilePath = database.GetMetaFilePath(aPath.generic_string().c_str());

		Assets::AssetMetaData d = database.GetMetaData(aPath.generic_string().c_str());
		const size_t guid = 0;

		std::filesystem::path artifactPath = GetArtifactPath(guid);

		bool fileExists = std::filesystem::exists(artifactPath);
		if (!fileExists || last_write_time(artifactPath) < last_write_time(aPath) ||
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
			Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
			metafilepath = database.GetMetaFilePath(aPath.generic_string().c_str());
		}

		std::ifstream in(metafilepath);
		cereal::JSONInputArchive archive(in);

		AssetMetaSettings metafile;
		archive(metafile);

		
		std::filesystem::path exportFolderPath = PathManager::GetProjectRoot() / "Local/Artifacts" / std::to_string(metafile.guid).substr(0, 2) / std::to_string(metafile.guid);

		auto time0 = std::filesystem::last_write_time(exportFolderPath).time_since_epoch().count();
		auto time1 = std::filesystem::last_write_time(metafilepath).time_since_epoch().count();

		return time0 == time1;
	}

	std::filesystem::path IEditorAssetImporter::GetArtifactPath(const size_t& aGuid)
	{
		std::filesystem::path folder = PathManager::GetArtifactsPath() / std::to_string(aGuid).substr(0, 2);
		std::filesystem::create_directories(folder);

		return folder / std::to_string(aGuid);
	}
}