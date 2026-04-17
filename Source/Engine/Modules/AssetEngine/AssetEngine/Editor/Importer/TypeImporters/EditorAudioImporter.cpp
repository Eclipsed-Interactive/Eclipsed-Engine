#include "EditorAudioImporter.h"

#include "AssetEngine/SupportedTypes.h"


namespace Eclipse
{
	void AudioImporter::Export(const std::string& guid, std::ofstream& outStream, const std::filesystem::path& aPath)
	{
        std::ifstream in(aPath, std::ios::binary | std::ios::ate);

        size_t size = in.tellg();

        in.seekg(0, std::ios::beg);

        std::vector<char> bytes(size);
        in.read(reinterpret_cast<char*>(bytes.data()), size);

        int type = (int)AssetType::AudioClip;
        outStream.write(reinterpret_cast<const char*>(&type), sizeof(int));

        outStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        outStream.write(bytes.data(), size);
	}
}