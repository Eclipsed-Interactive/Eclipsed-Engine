#pragma once

#include <vector>
#include "Core/GUID.h"
#include <Core/Standard/Core/Types.hpp>

namespace Eclipse::AssetSystem
{
	class IArtifactProvider
	{
	public:
		virtual ~IArtifactProvider() = default;

		virtual bool Contains(const GUID& aGuid) const = 0;
		virtual bool Read(const GUID& aGuid, std::vector<Byte>& outBytes) const = 0;
		virtual bool TryGetGuid(const std::string& aVirtualPath, GUID& outGuid) const = 0;
	};
}