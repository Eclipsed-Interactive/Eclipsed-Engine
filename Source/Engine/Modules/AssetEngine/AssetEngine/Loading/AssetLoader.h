#pragma once

#include <unordered_map>
#include <string>
#include <fstream>

#include "AssetEngine/Assets/Texture/TextureAsset.h"
#include "AssetEngine/Assets/Shader/ShaderAsset.h"
#include "AssetEngine/Assets/Material/MaterialAsset.h"
#include "AssetEngine/Assets/Prefab/PrefabAsset.h"
#include "AssetEngine/Assets/Font/FontAsset.h"
#include "AssetEngine/Assets/Audio/AudioAsset.h"

#include "AssetEngine/Loading/TypeLoaders/TextureLoader.h"
#include "AssetEngine/Loading/TypeLoaders/ShaderLoader.h"
#include "AssetEngine/Loading/TypeLoaders/MaterialLoader.h"
#include "AssetEngine/Loading/TypeLoaders/PrefabLoader.h"
#include "AssetEngine/Loading/TypeLoaders/AudioLoader.h"
#include "AssetEngine/Loading/TypeLoaders/FontLoader.h"

namespace Eclipse::Assets
{
	template<typename AssetType, typename Handle, typename Loader>
	class AssetManager;

	using ShaderManager = AssetManager<Shader, ShaderData, ShaderLoader>;
	using TextureManager = AssetManager<Texture, TextureData, TextureLoader>;
	using MaterialManager = AssetManager<Material, MaterialData, MaterialLoader>;
	using PrefabManager = AssetManager<Prefab, PrefabData, PrefabLoader>;
	using FontManager = AssetManager<Font, FontData, FontLoader>;
	using AudioClipManager = AssetManager<Audio, AudioData, AudioLoader>;

	template<typename AssetType, typename Handle, typename Loader>
	class AssetManager
	{
	public:
		AssetType Get(const GUID& aGuid, std::ifstream& in);

		void Update();
		bool CleanUp(const GUID& aGuid);

	protected:
		Loader& GetLoader();

		AssetType Load(const GUID& aGuid, std::ifstream& in);
		AssetType ConstructAsset(const GUID& guid);

	protected:
		std::unordered_map<GUID, Handle*, GUIDHash> idToAssetHandle;
	};

	template<typename AssetType, typename Handle, typename Loader>
	inline AssetType AssetManager<AssetType, Handle, Loader>::Get(const GUID& aGuid, std::ifstream& in)
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
	inline bool AssetManager<AssetType, Handle, Loader>::CleanUp(const GUID& aGuid)
	{
		if (idToAssetHandle.empty()) return false;

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
	inline AssetType AssetManager<AssetType, Handle, Loader>::Load(const GUID& aGuid, std::ifstream& in)
	{
		if (!in.is_open()) return {};

		idToAssetHandle[aGuid] = GetLoader().Load(in);
		idToAssetHandle[aGuid]->assetID = aGuid;
		return ConstructAsset(aGuid);
	}

	template<typename AssetType, typename Handle, typename Loader>
	inline AssetType AssetManager<AssetType, Handle, Loader>::ConstructAsset(const GUID& aGuid)
	{
		return AssetType(idToAssetHandle[aGuid]);
	}
}