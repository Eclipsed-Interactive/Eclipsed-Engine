#include "AudioClip.h"

#include <AssetSystem/Assets/Data/AudioData.hpp>
#include <Renderer/RendererManager.h>

namespace Eclipse
{
	AudioClip::AudioClip(AssetSystem::AssetData* aData, GUID aGuid)
		: myGuid(aGuid), myData(reinterpret_cast<AssetSystem::AudioData*>(aData))
	{
	}

	void* AudioClip::GetAudio() const
	{
		return myData->sound;
	}


	bool AudioClip::IsReady() const
	{
		return myData != nullptr;
	}
}
