#pragma once

#include <Core/GUID.h>
#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/RectSizePos.h"

#include "AssetSystem.Core.hpp"

namespace Eclipse
{
	namespace AssetSystem
	{
		struct SceneData;
		struct AssetData;
	}

	class ASSETSYSTEM_API Scene
	{
	public:
		Scene() = default;
		Scene(AssetSystem::AssetData* aData, GUID aGuid);

	public:
		constexpr static unsigned int Magic = 'ESCE';

	public:
		[[nodiscard]] std::string_view GetJson() const;

	public:
		[[nodiscard]] bool IsReady() const;
		[[nodiscard]] GUID GetGUID() const { return myGuid; }

	private:
		GUID myGuid;
		AssetSystem::SceneData* myData = nullptr;
	};
}