#include "AssetMetaData.h"

#include <string>
#include <fstream>

namespace Eclipse::Assets
{
	AssetMetaData::AssetMetaData(const char* aPath)
	{
		filePath = aPath;
		std::ifstream in(std::string(aPath) + ".meta");
		cereal::JSONInputArchive ar(in);

		ar(*this);
	}

	AssetMetaData::~AssetMetaData()
	{
		std::ofstream in(filePath + ".meta");
		cereal::JSONOutputArchive ar(in);

		ar(*this);
	}
}