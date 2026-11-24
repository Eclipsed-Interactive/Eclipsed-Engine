#include "ArtifactRegistry.h"
#include "IArtifactProvider.h"

namespace Eclipse::AssetSystem
{
	std::vector<IArtifactProvider*> ArtifactRegistry::providers;

	void ArtifactRegistry::Mount(IArtifactProvider* aProvider)
	{
		if (aProvider) providers.push_back(aProvider);
	}
	
	bool ArtifactRegistry::Contains(const GUID& aGuid)
	{
		return false;
	}
	
	bool ArtifactRegistry::Read(const GUID& aGuid, std::vector<Byte>& outBytes)
	{
		for (auto it = providers.begin(); it != providers.end(); ++it) 
		{
			if ((*it)->Read(aGuid, outBytes))
				return true;
		}

		outBytes.clear();
		return false;
	}

	bool ArtifactRegistry::TryGetGuid(const std::string& aVirtualPath, GUID& outGuid)
	{
		for (auto it = providers.begin(); it != providers.end(); ++it)
		{
			if ((*it)->TryGetGuid(aVirtualPath, outGuid))
				return true;
		}

		return false;
	}
}