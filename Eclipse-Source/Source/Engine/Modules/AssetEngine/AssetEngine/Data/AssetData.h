#pragma once

#include <string>

namespace Eclipse
{
	struct AssetData {
		std::string assetID = "";
		int refCount = 0;
	};
}