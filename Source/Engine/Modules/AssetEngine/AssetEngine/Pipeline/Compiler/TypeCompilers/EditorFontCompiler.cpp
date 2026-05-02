#include "EditorFontCompiler.h"

namespace Eclipse::Assets
{
	void FontCompiler::Internal_Compile(const AssetMeta& meta, std::vector<unsigned char>& binaryData)
	{
		const int type = (int)AssetType::Font;

		std::ifstream in(meta.fullPath, std::ios::binary | std::ios::ate);
		size_t size = in.tellg();

		in.seekg(0, std::ios::beg);

		std::vector<char> bytes(size);
		in.read(reinterpret_cast<char*>(bytes.data()), size);

		AddToBinaryData(&type, sizeof(type), binaryData);
		AddToBinaryData(&size, sizeof(size), binaryData);
		AddToBinaryData(bytes.data(), size, binaryData);
	}
}