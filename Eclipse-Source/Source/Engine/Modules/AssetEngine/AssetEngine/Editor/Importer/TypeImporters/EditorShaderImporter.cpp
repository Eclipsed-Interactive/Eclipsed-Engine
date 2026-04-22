#include "AssetEngine/Editor/Importer/TypeImporters/EditorShaderImporter.h"

#include "AssetEngine/Editor/MetaFile/AssetMetaManager.h"
#include "AssetEngine/Editor/MetaFile/MetaFileRegistry.h"
#include "AssetEngine/Editor/GUID/GuidGenerator.h"

#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"

#include <fstream>
#include <cereal/archives/json.hpp>

#include "AssetEngine/Data/ShaderData.h"
#include "AssetEngine/SupportedTypes.h"

#include "AssetEngine/Helper/ShaderCompiler.h"

namespace Eclipse
{
	template<AssetType Type>
	void ShaderImporter<Type>::Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath)
	{
		std::string shaderSource;
		{
			std::ifstream in(aPath);
			std::stringstream buffer;
			buffer << in.rdbuf();

			shaderSource = buffer.str();
		}

		int type = (int)Type;
		outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));

		size_t shadeSize = shaderSource.size();
		outStream.write(reinterpret_cast<const char*>(&shadeSize), sizeof(size_t));
		outStream.write(reinterpret_cast<const char*>(shaderSource.c_str()), shaderSource.size());
	}
}