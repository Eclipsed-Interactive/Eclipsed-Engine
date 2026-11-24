#pragma once

#include <Core/GUID.h>

#include "AssetSystem.Core.hpp"
#include "AssetSystem/ShaderType.h"

namespace Eclipse
{
	namespace AssetSystem
	{
		struct AssetData;
		struct ShaderData;
	}

	class ASSETSYSTEM_API Shader
	{
	public:
		Shader() = default;
		Shader(AssetSystem::AssetData* aData, GUID aGuid);

	public:
		constexpr static unsigned int Magic = 'ESHA';

	public:
		unsigned int ShaderID() const;
		AssetSystem::ShaderType Type() const;

	public:
		bool IsReady() const;
		[[nodiscard]] GUID GetGUID() const { return myGuid; }

	private:
		GUID myGuid;
		AssetSystem::ShaderData* myData = nullptr;
	};
}