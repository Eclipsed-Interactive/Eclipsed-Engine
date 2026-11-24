#pragma once 

#include <Core/GUID.h>

#include "IAssetLoader.h"
#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	class ASSETSYSTEM_API AssetLoaderRegistry
	{
	public:
		template<typename T>
		static void Register()
		{
			loaders.push_back(new T);
		}

		static const IAssetLoader* FindByMagic(unsigned int aMagic)
		{
			for (auto it = loaders.begin(); it != loaders.end(); ++it)
				if ((*it)->GetMagic() == aMagic) 
					return *it;

			return nullptr;
		}

	private:
		static inline std::vector<IAssetLoader*> loaders;
	};
}