#pragma once

#include <filesystem>
#include <Core/GUID.h>

#include <AssetSystem/Loading/AssetLoader.h>

#include "AssetSystem.Core.hpp"

#include "Assets/Types/Material.h"
#include "Assets/Types/Texture.h"
#include "Assets/Types/Shader.h"
#include "Assets/Types/Scene.h"

namespace Eclipse::Assets
{
	ASSETSYSTEM_API bool TryGetGuid(const std::filesystem::path& pathToAsset, GUID& outGuid);
	ASSETSYSTEM_API bool TryGetFull(GUID aGuid, std::filesystem::path& outPath);

	template<typename T>
	T Load(GUID aGuid)
	{
		return AssetSystem::AssetLoader::Load<T>(aGuid);
	}

	template<typename T>
	T Load(const char* aVirtualPath)
	{
		return AssetSystem::AssetLoader::Load<T>(aVirtualPath);
	}
}