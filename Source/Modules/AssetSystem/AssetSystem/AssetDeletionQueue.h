#pragma once

#include <unordered_map>
#include "Core/GUID.h"

namespace Eclipse::AssetSystem
{
	class AssetDeletionQueue
	{
	public:
		static void MarkForDelete(GUID guid);
		static void ProcessPendingForDelete();

	private:
		static inline std::vector<GUID> pendingForDelete;
	};
}