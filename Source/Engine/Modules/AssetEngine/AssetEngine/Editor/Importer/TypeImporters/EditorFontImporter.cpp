#include "AssetEngine/Editor/Importer/TypeImporters/EditorFontImporter.h"

#include <fstream>
#include "AssetEngine/Data/FontData.h"
#include "AssetEngine/SupportedTypes.h"

namespace Eclipse
{
	void FontImporter::Export(const size_t& guid, std::ofstream& outStream, const std::filesystem::path& aPath)
	{
		std::ifstream in(aPath, std::ios::binary);
		size_t size = file_size(aPath);

		char* fontData = static_cast<char*>(malloc(size));
		in.read(fontData, size);

		int type = (int)AssetType::Font;
		outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(&size), sizeof(int));
		outStream.write(fontData, size);
	}
}