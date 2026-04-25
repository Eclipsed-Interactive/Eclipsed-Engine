#include "EditorMaterialImporter.h"

#include "AssetEngine/Editor/GUID/GuidGenerator.h"

#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"

#include <fstream>
#include <cereal/archives/json.hpp>

#include "AssetEngine/Data/MaterialData.h"
#include "AssetEngine/SupportedTypes.h"

namespace Eclipse
{
	void MaterialImporter::Export(const size_t& guid, std::ofstream& outStream, const std::filesystem::path& aPath)
	{
		SerializedMaterial data = ValidateJsonFile<SerializedMaterial>(aPath);

		int type = (int)AssetType::Material;
		outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));

		outStream.write(reinterpret_cast<const char*>(&data.textureGuid), sizeof(size_t));
		outStream.write(reinterpret_cast<const char*>(&data.pixelShaderGuid), sizeof(size_t));
		outStream.write(reinterpret_cast<const char*>(&data.vertexShaderGuid), sizeof(size_t));
	}
}