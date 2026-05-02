#include "EditorTextureCompiler.h"

#include "AssetEngine/Pipeline/MetaData/Assetmeta.h"
#include "AssetEngine/Pipeline/MetaData/Data/TextureMeta.h"

#include <fstream>
#include "AssetEngine/Helper/STB_Helper.h"
#include "AssetEngine/BinaryFormats/TextureData.h"
#include "AssetEngine/SupportedAssets.h"
#include "CoreEngine/MainSingleton.h"
#include "AssetEngine/Pipeline/AssetDataBase.h"

namespace Eclipse::Assets
{
	void TextureCompiler::Internal_Compile(const AssetMeta& meta, std::vector<unsigned char>& binaryData)
	{
		const int type = (int)AssetType::Texture;

		int width = 0, height = 0, channels = 0;
		unsigned char* pixelData;
		pixelData = STB_Helper::Load_Texture_STB(meta.fullPath.generic_string().c_str(), width, height, channels, true);

		AddToBinaryData(&type, sizeof(type), binaryData);

		AddToBinaryData(&width, sizeof(width), binaryData);
		AddToBinaryData(&height, sizeof(height), binaryData);
		AddToBinaryData(&channels, sizeof(channels), binaryData);
		AddToBinaryData(pixelData, width * height * channels, binaryData);

		STB_Helper::FreeData_STB(pixelData);

		const TextureMeta* textureMeta = meta.GetMetaComponent<TextureMeta>();
		const int dataAmount = textureMeta->spriteRects.size();
		AddToBinaryData(&dataAmount, sizeof(dataAmount), binaryData);
		AddToBinaryData(textureMeta->spriteRects.data(), dataAmount, binaryData);
	}
}