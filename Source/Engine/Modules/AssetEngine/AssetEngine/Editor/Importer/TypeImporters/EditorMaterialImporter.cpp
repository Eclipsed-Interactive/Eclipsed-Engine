#include "EditorMaterialImporter.h"

#include "AssetEngine/Editor/MetaFile/AssetMetaManager.h"
#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"
#include "AssetEngine/Editor/GUID/GuidGenerator.h"

#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"

#include <fstream>
#include <cereal/archives/json.hpp>

#include "AssetEngine/Data/MaterialData.h"
#include "AssetEngine/SupportedTypes.h"

namespace Eclipse
{
	void MaterialImporter::Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath)
	{
		SerializedMaterial data = ValidateJsonFile<SerializedMaterial>(aPath);

		int type = (int)AssetType::Material;
		outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));

		const int textureNameSize = data.textureGuid.size();
		const int pixelShaderNameSize = data.pixelShaderGuid.size();
		const int vertexShaderNameSize = data.vertexShaderGuid.size();

		outStream.write(reinterpret_cast<const char*>(&textureNameSize), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(&pixelShaderNameSize), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(&vertexShaderNameSize), sizeof(int));

		outStream.write(data.textureGuid.c_str(), data.textureGuid.size());
		outStream.write(data.pixelShaderGuid.c_str(), data.pixelShaderGuid.size());
		outStream.write(data.vertexShaderGuid.c_str(), data.vertexShaderGuid.size());
	}
}