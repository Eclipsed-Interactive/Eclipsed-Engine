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
		Scene(AssetSystem::AssetData* data);

	public:
		constexpr static unsigned int Magic = 'ESCE';

	public:
		[[nodiscard]] std::string_view GetJson() const;
		[[nodiscard]] GUID GetGUID() const;

	public:
		[[nodiscard]] bool IsReady() const;

	private:
		GUID myGuid;
		AssetSystem::SceneData* myData = nullptr;
	};
}