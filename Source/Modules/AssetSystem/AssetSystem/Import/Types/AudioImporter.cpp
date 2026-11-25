#include "AudioImporter.h"

#include "AssetSystem/Import/ImportContext.h"
#include <Audio/AudioPluginLoader.h>


namespace Eclipse::AssetSystem
{
	bool AudioImporter::Import(ImportContext& ctx) const
	{
		ArtifactWriter& writer = ctx.Out();

		std::ifstream file(ctx.GetPath(), std::ios::binary |std::ios::ate);
		if (!file) return false;

		Audio::IAudioBackend* backend = Audio::AudioManager::GetBackend();

		Audio::SoundInfo info;

		const std::filesystem::path path = ctx.GetPath();
		const std::string str = path.generic_string();

		backend->GetSoundInfo(str.c_str(), &info);

		const Size size = file.tellg();
		file.seekg(0);

		std::vector<Byte> bytes(size);
		file.read(reinterpret_cast<char*>(bytes.data()), size);

		writer.Write(&size, sizeof(Size));
		writer.Write(reinterpret_cast<const char*>(bytes.data()), size);
		
		return true;
	}
}