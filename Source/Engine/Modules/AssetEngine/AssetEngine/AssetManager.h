#pragma once

#include "AssetEngine/Loading/AssetLoader.h"

#include "Assets/Asset.h"
#include "Assets/Audio/AudioAsset.h"
#include "Assets/Font/FontAsset.h"
#include "Assets/Material/MaterialAsset.h"
#include "Assets/Prefab/PrefabAsset.h"
#include "Assets/Shader/ShaderAsset.h"
#include "Assets/Texture/TextureAsset.h"

#include "GUID.h"




namespace Eclipse::Assets
{
	class Resources
	{
	public:
		template<typename T>
		T Get(GUID guid);



	private:
		static ShaderManager shaderManager;
		static TextureManager  textureManager;
		static MaterialManager  materialManager;
		static PrefabManager prefabManager;
		static FontManager  fontManager;
		static AudioClipManager audioClipManager;
	};

	template<typename T>
	inline T Resources::Get(GUID guid)
	{
		if constexpr(std::is_same<T, Texture>::value) return textureManager.Load()

		return T();
	}
}