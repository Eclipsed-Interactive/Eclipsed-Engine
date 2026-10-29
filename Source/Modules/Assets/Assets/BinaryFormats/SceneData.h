#pragma once

#include "AssetData.h"
#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/RectSizePos.h"

namespace Eclipse::Assets
{
	struct SceneData : public AssetData {
		~SceneData();

		std::string sourceBlob;
		std::string name;
	};
}