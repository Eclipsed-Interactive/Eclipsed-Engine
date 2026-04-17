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
		void Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath) override;
	};
}