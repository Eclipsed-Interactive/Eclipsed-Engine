#pragma once

#include "IAssetImporter.h"
#include "ImporterEntry.h"
#include "AssetSystem/AssetTypes.h"
namespace Eclipse::AssetSystem
{
	class ImporterRegistry
	{
	public:
		template<typename T>
		static void Register(AssetType aType, std::vector<std::string> someSupportedExtensions);

		static const IAssetImporter* FindByExtension(std::string_view anExtension);
		static const IAssetImporter* FindByType(TypeID aType);

	private:
		static std::vector<ImporterEntry> importers;
	};

	template<typename T>
	inline void ImporterRegistry::Register(AssetType aType, std::vector<std::string> someSupportedExtensions)
	{
		importers.emplace_back(
			Type::GetTypeID<T>(),
			std::move(someSupportedExtensions),
			new T
		);
	}
}