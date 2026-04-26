#include "AssetEngine/Editor/Importer/TypeImporters/EditorTextureImporter.h"

#include "AssetEngine/Editor/Importer/ImportSettings/AssetImportSettings.h"

#include <fstream>
#include "AssetEngine/Helper/STB_Helper.h"
#include "AssetEngine/Data/TextureData.h"
#include "AssetEngine/SupportedTypes.h"
#include "CoreEngine/MainSingleton.h"
#include "AssetEngine/AssetDatabase.h"

namespace Eclipse
{
	void TextureImporter::Export(const size_t& guid, std::ofstream& outStream, const std::filesystem::path& aPath)
	{
		TextureData data;
		
		unsigned char* pixelData;
		pixelData = STB_Helper::Load_Texture_STB(aPath.generic_string().c_str(), data.width, data.height, data.channels, true);
		
		int type = (int)AssetType::Texture;
		outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(&data.width), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(&data.height), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(&data.channels), sizeof(int));
		

		outStream.write(reinterpret_cast<const char*>(pixelData), data.width * data.height * data.channels);
		STB_Helper::FreeData_STB(pixelData);



		const Assets::AssetMetaData& meta = MainSingleton::GetInstance<Assets::AssetDatabase>().GetMetaData(aPath.generic_string().c_str());
		int dataAmount = meta.spriteRects.size();
		outStream.write(reinterpret_cast<const char*>(&dataAmount), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(meta.spriteRects.data()), dataAmount);
	}
}