#pragma once

#include <Core/Standard/TypeIDGenerator.hpp>

#include <vector>
#include <string>

namespace Eclipse::AssetSystem
{
	class IAssetImporter;
	struct ImporterEntry
	{
		TypeID type;
		std::vector<std::string> extensions;
		IAssetImporter* importer;
	};
}