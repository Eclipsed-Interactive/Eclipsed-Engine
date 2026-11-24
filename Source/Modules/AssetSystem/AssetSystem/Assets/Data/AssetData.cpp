#include "AssetData.h"

#include "AssetDeletionQueue.h"

namespace Eclipse::AssetSystem
{
	void AssetData::IncreaseRefCount()
	{
		refCount++;
	}

	void AssetData::DecreaseRefCount()
	{
		if (refCount <= 0)
			return;

		--refCount;

		if (refCount == 0)
		{
			AssetDeletionQueue::MarkForDelete(guid);
		}
	}
}