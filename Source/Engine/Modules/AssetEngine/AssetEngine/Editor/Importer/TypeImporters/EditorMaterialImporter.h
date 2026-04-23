#pragma once

#include "AssetEngine/Editor/Importer/IEditorAssetImporter.h"
#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"
#include <fstream>
#include <filesystem>

namespace Eclipse
{
	class MaterialImporter : public IEditorAssetImporter
	{
	public:
		void Export(const size_t& guid, std::ofstream& outStream, const std::filesystem::path& aPath) override;
	};
}