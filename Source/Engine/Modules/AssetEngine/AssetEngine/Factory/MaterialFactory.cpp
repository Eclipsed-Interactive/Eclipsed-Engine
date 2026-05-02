#include "MaterialFactory.h"

#include <fstream>
#include <filesystem>

#include "CoreEngine/Math/Color.h"

#include "AssetEngine/BinaryFormats/MaterialData.h"

namespace Eclipse::Assets
{
	void MaterialFactory::CreateNew(const char* materialName, const char* folderPath)
	{
		std::ofstream out(std::filesystem::path(folderPath) / materialName);
		cereal::JSONOutputArchive ar(out);

		MaterialData data{};
		ar(data);
	}
}