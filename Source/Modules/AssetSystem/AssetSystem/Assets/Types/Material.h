#pragma once

#include <Core/GUID.h>
#include "AssetSystem/Assets/Types/Texture.h"
#include "AssetSystem/Assets/Types/Shader.h"
#include <Core/Math/Color.h>

#include "AssetSystem.Core.hpp"

namespace Eclipse
{
	namespace AssetSystem
	{
		struct MaterialData;
	}

	class Texture;
	class Shader;
	class ASSETSYSTEM_API Material
	{
	public:
		Material() = default;
		Material(AssetSystem::AssetData* aData, GUID aGuid);

	public:
		constexpr static unsigned int Magic = 'EMAT';

	public:
		const Math::Color& Color() const;
		Texture GetTexture() const;
		Shader GetPixelShader() const;
		Shader GetVertexShader() const;

	public:
		bool IsReady() const;
		[[nodiscard]] GUID GetGUID() const { return myGuid; }

		void Bind();

	private:
		GUID myGuid;
		AssetSystem::MaterialData* myData = nullptr;
	};
}