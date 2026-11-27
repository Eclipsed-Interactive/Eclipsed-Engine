#include "AudioLoader.h"

#include <AssetSystem/Assets/Data/AudioData.hpp>
#include <Core/Standard/Core/Types.hpp>
#include "ArtifactHeader.h"

#include "AssetSystem/Loading/AssetLoader.h"

#include <Audio/AudioPluginLoader.h>

namespace Eclipse::AssetSystem
{
	bool AudioLoader::Load(ArtifactReader& aReader, AssetData* aData) const
	{
		AudioData* aud = static_cast<AudioData*>(aData);

		Size size = 0;
		aReader.Read(&size, sizeof(Size));

		std::vector<Byte> data(size);
		aReader.Read(data.data(), size);

		Audio::SoundInfo info;

		Audio::IAudioBackend* backend = Audio::AudioManager::GetBackend();
		aud->sound = backend->CreateSound(data.data(), size, &info);

		aud->channels = info.channels;
		aud->sampleRate = info.sampleRate;
		aud->lengthSeconds = info.lengthSeconds;

		return true;
	}
	bool AudioLoader::Unload(AssetData* aData) const
	{
		return false;
	}
}