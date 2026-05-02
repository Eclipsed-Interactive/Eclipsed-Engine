#include "AssetData.h"

namespace Eclipse::Assets
{
	void AssetData::IncreaseRefCount()
	{
		refCount++;
	}
	void AssetData::DecreaseRefCount()
	{
		refCount--;
		if (refCount == 0)
		{
			// AssetEngine.MarkForDelete();
		}
	}
}