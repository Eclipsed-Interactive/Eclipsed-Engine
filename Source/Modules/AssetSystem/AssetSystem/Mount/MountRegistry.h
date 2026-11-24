#pragma once

#include <string>
#include <vector>
#include "MountEntry.h"

namespace Eclipse::AssetSystem
{
	class MountRegistry
	{
	public:
		template<typename MountType>
		static void Mount(const std::string& aScheme, const std::string& aMountPath);

	private:
		static std::vector<MountEntry> mounts;
	};

	template<typename MountType>
	inline void MountRegistry::Mount(const std::string& aScheme, const std::string& aMountPath)
	{
		IMount* entry = new IMount(aScheme, aMountPath);

		mounts.emplace(
			aScheme,
			entry
		);
	}
}