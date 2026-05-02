#pragma once

#include <string>
#include "AssetEngine/GUID.h"

namespace Eclipse::Assets
{
	struct AssetData {
		int refCount = 0;
		GUID guid;

		void IncreaseRefCount();
		void DecreaseRefCount();
	};
}