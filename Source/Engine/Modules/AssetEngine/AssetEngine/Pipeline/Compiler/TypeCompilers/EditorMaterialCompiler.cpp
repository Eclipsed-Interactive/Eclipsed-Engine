#include "EditorMaterialCompiler.h"

#include<sstream>
#include<fstream>
#include "AssetEngine/BinaryFormats/MaterialData.h"

namespace Eclipse::Assets
{
	void MaterialCompiler::Internal_Compile(const AssetMeta& meta, std::vector<unsigned char>& binaryData)
	{
		std::ifstream in(meta.fullPath);
		cereal::JSONInputArchive ar(in);

		MaterialData data{};
		ar(data);

		const int type = (int)AssetType::Material;
		AddToBinaryData(&type, sizeof(type), binaryData);


		std::string shaderGuid = data.shader.guid.ToString();
		std::string textureGuid = data.texture.guid.ToString();
		AddToBinaryData(shaderGuid.data(), shaderGuid.size(), binaryData);
		AddToBinaryData(textureGuid.data(), textureGuid.size(), binaryData);
		AddToBinaryData(&data.color, sizeof(data.color), binaryData);
	}
}