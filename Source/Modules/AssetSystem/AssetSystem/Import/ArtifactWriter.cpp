#include "ArtifactWriter.h"

#include <fstream>

namespace Eclipse::AssetSystem
{
	void ArtifactWriter::Write(const void* data, Size size)
	{
		if (size == 0) return;

		const Size offset = bytes.size();
		bytes.resize(offset + size);
		std::memcpy(bytes.data() + offset, data, size);
	}

	void ArtifactWriter::SaveToFile(const std::filesystem::path& aPath)
	{
		std::filesystem::create_directories(aPath.parent_path());
		std::ofstream file(aPath, std::ios::binary);

		file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
	}


	std::span<const Byte> ArtifactWriter::GetBytes() const
	{
		return bytes;
	}

}