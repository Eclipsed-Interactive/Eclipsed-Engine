#pragma once

#include <assert.h>

#include "AssetEngine/Core/GUID.h"

#include "CoreEngine/Settings/SettingsBase.hpp"

#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"

namespace Eclipse::Assets
{
	template<typename T>
	struct Asset {
		virtual ~Asset();
		virtual bool IsValid() const {
			assert("Not implemented");
			return false;
		}
		
		GUID guid;
		T* dataPtr = nullptr;

		GUID GetAssetID() const;


		template <class Archive> 
		void serialize(Archive& ar);
	};

	template<typename T>
	inline Asset<T>::~Asset()
	{
		dataPtr->DecreaseRefCount();
	}

	template<typename T>
	inline GUID Asset<T>::GetAssetID() const
	{
		return guid;
	}

	template<typename T>
	template<class Archive>
	inline void Asset<T>::serialize(Archive& ar)
	{
		std::string assetGuid = guid.ToString();

		ar(
			cereal::make_nvp("guid", assetGuid)
		);

		guid.FromString(assetGuid);
	}
}