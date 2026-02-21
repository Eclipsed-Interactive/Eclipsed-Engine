#pragma once

#include <unordered_map>
#include <string>
#include <fstream>

#include "AssetEngine/Assets/Texture.h"
#include "AssetEngine/Assets/VertexShader.h"
#include "AssetEngine/Assets/PixelShader.h"
#include "AssetEngine/Assets/Material.h"
#include "AssetEngine/Assets/Prefab.h"
#include "AssetEngine/Assets/Audioclip.h"

#include "AssetEngine/Runtime/Loader/TypeLoaders/TextureLoader.h"
#include "AssetEngine/Runtime/Loader/TypeLoaders/ShaderLoader.h"
#include "AssetEngine/Runtime/Loader/TypeLoaders/MaterialLoader.h"
#include "AssetEngine/Runtime/Loader/TypeLoaders/PrefabLoader.h"
#include "AssetEngine/Runtime/Loader/TypeLoaders/AudioLoader.h"

namespace Eclipse
{
	template<typename AssetType, typename Handle, typename Loader>
	class AssetManager;

	using VertexShaderManager = AssetManager<VertexShader, VertexShaderData, VertexShaderLoader>;
	using PixelShaderManager = AssetManager<PixelShader, PixelShaderData, PixelShaderLoader>;
	using TextureManager = AssetManager<Texture, TextureData, TextureLoader>;
	using MaterialManager = AssetManager<Material, MaterialData, MaterialLoader>;
	using PrefabManager = AssetManager<Prefab, PrefabData, PrefabLoader>;
	using AudioClipManager = AssetManager<AudioClip, AudioClipData, AudioLoader>;

	template<typename AssetType, typename Handle, typename Loader>
	class AssetManager
	{
	public:
		AssetType Get(const std::string& aGuid, std::ifstream& in);

		void Update();
		bool CleanUp(const std::string& aGuid);

	protected:
		Loader& GetLoader();

		AssetType Load(const std::string& aGuid, std::ifstream& in);
		AssetType ConstructAsset(const std::string& guid);

	protected:
		std::unordered_map<std::string, Handle*> idToAssetHandle;
	};

	template<typename AssetType, typename Handle, typename Loader>
	inline AssetType AssetManager<AssetType, Handle, Loader>::Get(const std::string& aGuid, std::ifstream& in)
	{
		if (idToAssetHandle.find(aGuid) != idToAssetHandle.end()) return ConstructAsset(aGuid);
		else return Load(aGuid, in);
	}

	template<typename AssetType, typename Handle, typename Loader>
	void AssetManager<AssetType, Handle, Loader>::Update()
	{
		for (auto it = idToAssetHandle.begin(); it != idToAssetHandle.end();)
		{
			Handle* handle = it->second;

			if (!handle)
			{
				it = idToAssetHandle.erase(it);
				continue;
			}

			if (handle->refCount <= 0)
			{
				if (handle) delete handle;

				it = idToAssetHandle.erase(it);
				continue;
			}

			it++;
		}
	}

	template<typename AssetType, typename Handle, typename Loader>
	inline bool AssetManager<AssetType, Handle, Loader>::CleanUp(const std::string& aGuid)
	{
		auto it = idToAssetHandle.find(aGuid);
		if (it != idToAssetHandle.end())
		{
			delete idToAssetHandle[aGuid];
			idToAssetHandle.erase(aGuid);

			return true;
		}

		return false;
	}

	template<typename AssetType, typename Handle, typename Loader>
	inline Loader& AssetManager<AssetType, Handle, Loader>::GetLoader()
	{
		static Loader loader{};
		return loader;
	}

	template<typename AssetType, typename Handle, typename Loader>
	inline AssetType AssetManager<AssetType, Handle, Loader>::Load(const std::string& aGuid, std::ifstream& in)
	{
		if (!in.is_open()) return {};

		idToAssetHandle[aGuid] = GetLoader().Load(in);
		idToAssetHandle[aGuid]->assetID = aGuid;
		return ConstructAsset(aGuid);
	}

	template<typename AssetType, typename Handle, typename Loader>
	inline AssetType AssetManager<AssetType, Handle, Loader>::ConstructAsset(const std::string& aGuid)
	{
		return AssetType(idToAssetHandle[aGuid]);
	}
}