#pragma once

#include "AssetEngine/Editor/Importer/IEditorAssetImporter.h"

namespace Eclipse
{
	class FontImporter : public IEditorAssetImporter
	{
	public:
		void Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath) override;
	};
}