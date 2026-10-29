#include "AssetDeletionQueue.h"

#include "AssetCache.h"

namespace Eclipse::Assets
{
	void AssetDeletionQueue::MarkForDelete(GUID guid)
	{
		pendingForDelete.push_back(guid);
	}

	void AssetDeletionQueue::ProcessPendingForDelete()
	{
		if (pendingForDelete.empty()) return;

		for (GUID& guid : pendingForDelete)
		{
			AssetData* data = AssetCache::Get(guid);

			if (data == nullptr) continue;

			if (data->refCount <= 0)
			{
				AssetCache::Delete(guid);
			}
		}

		pendingForDelete.clear();
	}
}