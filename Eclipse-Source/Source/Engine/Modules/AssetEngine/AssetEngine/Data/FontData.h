#pragma once

#include "AssetData.h"

#include "CoreEngine/FontData.h"

namespace Eclipse
{
	class GameObject;

	struct FontData : public AssetData {
		InternalFont font;

		//void LoadNewFontSize();
	};
}
