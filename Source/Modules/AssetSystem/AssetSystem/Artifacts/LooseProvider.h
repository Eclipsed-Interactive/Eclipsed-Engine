#pragma once

#include "IArtifactProvider.h"
#include <AssetSystem/Index.h>

#include <filesystem>

namespace Eclipse::AssetSystem
{
	class LooseProvider : public IArtifactProvider
	{
	public:
		explicit LooseProvider(const std::filesystem::path& anArtifactLocation)
			: artifactLocation(anArtifactLocation) { }

	public:
		ArtifactIndex& Index();
		bool Contains(const GUID& aGuid) const override;
		bool Read(const GUID& aGuid, std::vector<Byte>& outBytes) const override;
		bool TryGetGuid(const std::string& aVirtualPath, GUID& outGuid) const override;

	private:
		std::filesystem::path artifactLocation;
		ArtifactIndex index{};
	};
}