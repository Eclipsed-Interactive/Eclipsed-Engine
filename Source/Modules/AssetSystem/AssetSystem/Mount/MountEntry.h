#pragma once

#include "Types/IMount.h"

namespace Eclipse::AssetSystem
{
	struct MountEntry
	{
		std::string scheme;
		IMount* mount;
	};
}