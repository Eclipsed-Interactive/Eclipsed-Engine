#pragma once

#include <Core/GUID.h>
#include "AssetSystem.Core.hpp"

namespace Eclipse
{
	namespace AssetSystem
	{
		struct AudioData;
		struct AssetData;
	}

	class ASSETSYSTEM_API AudioClip
	{
	public:
		AudioClip() = default;
		AudioClip(AssetSystem::AssetData* aData, GUID aGuid);

	public:
		constexpr static unsigned int Magic = 'EAUD';

	public:
		void* GetAudio() const;

	public:
		[[nodiscard]] bool IsReady() const;
		[[nodiscard]] GUID GetGUID() const { return myGuid; }

	private:
		GUID myGuid;
		AssetSystem::AudioData* myData = nullptr;
	};
}