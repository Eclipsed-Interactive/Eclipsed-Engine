#pragma once

#include <unordered_map>
#include "Core/SupportedAssets.h"

#include "Assets/Assets/AudioAsset.h"
#include "Assets/Assets/FontAsset.h"
#include "Assets/Assets/MaterialAsset.h"
#include "Assets/Assets/PrefabAsset.h"
#include "Assets/Assets/SceneAsset.h"
#include "Assets/Assets/TextureAsset.h"
#include "Assets/Assets/Shader/PixelShaderAsset.h"
#include "Assets/Assets/Shader/VertexShaderAsset.h"

#include "Assets.Core.hpp"

namespace Eclipse::Assets
{
	class IAssetType;
	class ASSET_API AssetTypeRegistry
	{
	public:
		
		static void RegisterTypes();
		static void RegisterType(AssetType type, IAssetType* instance);

		static  IAssetType* GetType(AssetType assetType);

		template<typename T>
		static IAssetType* GetType();

	private:
		static inline std::unordered_map<AssetType, IAssetType*> types;
	};

	template<typename T>
	inline IAssetType* AssetTypeRegistry::GetType()
	{
		if constexpr (std::is_same<T, AudioClip>::value)return types[AssetType::AudioClip];
		else if constexpr (std::is_same<T, Font>::value)return types[AssetType::Font];
		else if constexpr (std::is_same<T, Material>::value)return types[AssetType::Material];
		else if constexpr (std::is_same<T, Prefab>::value)return types[AssetType::Prefab];
		else if constexpr (std::is_same<T, VertexShader>::value)return types[AssetType::VertexShader];
		else if constexpr (std::is_same<T, PixelShader>::value)return types[AssetType::PixelShader];
		else if constexpr (std::is_same<T, Texture>::value)return types[AssetType::Texture];
		else if constexpr (std::is_same<T, Scene>::value)return types[AssetType::Scene];

		return nullptr;
	}
}