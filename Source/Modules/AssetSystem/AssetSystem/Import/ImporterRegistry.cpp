#include "ImporterRegistry.h"

namespace Eclipse::AssetSystem
{
	std::vector<ImporterEntry> ImporterRegistry::importers;

	const IAssetImporter* ImporterRegistry::FindByExtension(std::string_view anExtension)
	{
		for (auto entry : importers)
			if (std::find(entry.extensions.begin(), entry.extensions.end(), anExtension) != entry.extensions.end())
				return entry.importer;

		return nullptr;
	}

	const IAssetImporter* ImporterRegistry::FindByType(TypeID aType)
	{
		for (auto entry : importers)
			if (entry.type == aType)
				return entry.importer;

		return nullptr;
	}
}