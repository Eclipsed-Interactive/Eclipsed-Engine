#pragma once

#include "../../GUID.h"

#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include <fstream>

#include "Data/IAssetMeta.h"
#include "AssetEngine/SupportedAssets.h"

#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"

namespace Eclipse::Assets
{
	struct AssetMeta
	{
		~AssetMeta();

		template<typename T, typename... Args>
		T* AddMetaComponent(Args... args);

		template<typename T>
		const T* GetMetaComponent() const;

		template<typename T>
		bool HasMetaComponent()const;

		void WriteToStream(std::ofstream& _of) const;
		void LoadFromStream(std::ifstream& _if);

		void WriteToStreamBinary(std::ofstream& _of) const;
		void LoadFromStreamBinary(std::ifstream& _if);

		std::string GetArtifactPath() const;

		GUID guid;
		std::filesystem::path fullPath = "";
		std::filesystem::path exportedPath = "";
		AssetType type = AssetType::Unknown;

	private:
		std::unordered_map<std::type_index, IAssetMeta*> metaComponents;

	public:
		template <class Archive>
		void serialize(Archive& ar, const std::uint32_t version);
	};



	template<typename T, typename ...Args>
	inline T* AssetMeta::AddMetaComponent(Args ...args)
	{
		metaComponents[typeid(T)] = new T(std::forward<Args>(args)...);
		return reinterpret_cast<T*>(metaComponents[typeid(T)]);
	}

	template<typename T> 
	inline const T* AssetMeta::GetMetaComponent() const
	{
		if (!HasMetaComponent<T>()) return nullptr;

		return reinterpret_cast<const T*>(metaComponents.at(typeid(T)));
	}

	template<typename T>
	inline bool AssetMeta::HasMetaComponent() const
	{
		return metaComponents.find(typeid(T)) != metaComponents.end();
	}

	template<class Archive>
	inline void AssetMeta::serialize(Archive& ar, const std::uint32_t version)
	{
		std::string guidStr = guid.ToString();

		ar(
			CEREAL_NVP(guidStr),
			CEREAL_NVP(type)
		);

		guid.FromString(guidStr);

		for (auto& [typeIdx, comp] : metaComponents)
		{
			std::string componentKey = typeIdx.name();
			ar(cereal::make_nvp(componentKey, *comp));
		}
	}
}