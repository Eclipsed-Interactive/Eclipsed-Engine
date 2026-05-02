#include "EditorPrefabCompiler.h"

#include "AssetEngine/BinaryFormats/PrefabData.h"

namespace Eclipse::Assets
{
	void PrefabCompiler::Internal_Compile(const AssetMeta& meta, std::vector<unsigned char>& binaryData)
	{
		PrefabData data;
		{
			std::ifstream in(meta.fullPath);
			size_t prefSize = std::filesystem::file_size(meta.fullPath);

			data.data = reinterpret_cast<char*>(malloc(prefSize + 1));
			in.read(data.data, prefSize);
			memset(data.data + prefSize, '\0', 1);
		}

		const int size = strlen(data.data);

		int type = (int)AssetType::Prefab;
		AddToBinaryData(&type, sizeof(type), binaryData);
		AddToBinaryData(&size, sizeof(size), binaryData);
		AddToBinaryData(data.data, size, binaryData);
	}
}