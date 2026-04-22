#pragma once

#include <filesystem>
#include <fstream>
#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"
#include <cereal/archives/json.hpp>

#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"
#include "AssetEngine/Editor/GUID/GuidGenerator.h"

namespace Eclipse
{
	class IEditorAssetImporter
	{
	public:
		virtual ~IEditorAssetImporter() = default;

		virtual bool CanImport(const std::filesystem::path& aPath) const;

		void Import(const std::filesystem::path& aPath);

		virtual void Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath) = 0;

		bool ArtifactUpToData(const std::filesystem::path& aPath);

	protected:
		std::filesystem::path GetArtifactPath(const std::string& guid);

		template<typename T = AssetMetaSettings>
		T LoadOrCreateMeta(const std::filesystem::path& aPath);

		template<typename T>
		T ValidateJsonFile(const std::filesystem::path& aPath);
	};

	template<typename T>
	inline T IEditorAssetImporter::LoadOrCreateMeta(const std::filesystem::path& aPath)
	{
		T settings;

		std::filesystem::path metaFilePath = MetaFileRegistry::GetMetaFilePath(aPath);
		if (std::filesystem::exists(metaFilePath))
		{
			
			try
			{
				std::ifstream in(metaFilePath);
				cereal::JSONInputArchive ar(in);
				ar(settings);
			}
			catch(cereal::Exception e)
			{
				AssetMetaSettings tempSettings;

				std::ifstream in(metaFilePath);
				cereal::JSONInputArchive ar(in);
				ar(tempSettings);
				settings.guid = tempSettings.guid;
			}
		}
		else
		{
			MetaFileRegistry::CreateMetaFile(aPath);
			settings.guid = GuidGenerator::Generate();

			std::ofstream metafile(metaFilePath);
			cereal::JSONOutputArchive ar(metafile);
			ar(settings);
		}

		return settings;
	}

	template<typename T>
	inline T IEditorAssetImporter::ValidateJsonFile(const std::filesystem::path& aPath)
	{
		T data;

		try
		{
			std::ifstream in(aPath);
			cereal::JSONInputArchive ar(in);
			ar(data);
		}
		catch (const cereal::Exception& e)
		{
			std::ofstream in(aPath);
			cereal::JSONOutputArchive ar(in);
			ar(data);
		}

		return data;
	}
}