#pragma once

#include "AssetData.h"

#include "CoreEngine/FontData.h"

namespace Eclipse::Assets
{
	struct FontData : public AssetData {
		InternalFont font;

		//void LoadNewFontSize();
	};
}
