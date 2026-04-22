#include "EditorPrefabImporter.h"

#include "AssetEngine/Data/PrefabData.h"

#include "AssetEngine/SupportedTypes.h"

namespace Eclipse
{
	void PrefabImporter::Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath)
	{
		PrefabData data;
		{
			std::ifstream in(aPath);
			size_t prefSize = std::filesystem::file_size(aPath);

			data.data = reinterpret_cast<char*>(malloc(prefSize + 1));
			in.read(data.data, prefSize);
			memset(data.data + prefSize, '\0', 1);
		}

		int size = strlen(data.data);

		int type = (int)AssetType::Prefab;
		outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(&size), sizeof(int));
		outStream.write(reinterpret_cast<const char*>(data.data), size);
	}
}