#include "Index.h"

namespace Eclipse::AssetSystem
{
	void ArtifactIndex::Add(const GUID& aGuid, const AssetPath& aPath)
	{
		pathToGuid[aPath.Get()] = aGuid;
		guidToPath[aGuid] = aPath;
	}

	void ArtifactIndex::Remove(const GUID& aGuid)
	{
	}

	void ArtifactIndex::Clear()
	{
		guidToPath.clear();
		pathToGuid.clear();
	}

	bool ArtifactIndex::TryGetGuid(const std::string& aPath, GUID& outGuid) const
	{
		auto it = pathToGuid.find(aPath);
		if(it == pathToGuid.end())
			return false;

		outGuid = it->second;
		return  true;
	}

	const AssetPath* ArtifactIndex::TryGetPath(const GUID& aGuid) const
	{
		auto it = guidToPath.find(aGuid);
		return it != guidToPath.end() ? &it->second : nullptr;
	}
}