#include "FileSystem.h"

#include <fstream>

namespace Eclipse::Assets
{
	std::vector<FileEntry> FileSystem::ScanDirectory(const std::filesystem::path& root)
	{
		std::vector<FileEntry> result;

		for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (!entry.is_regular_file())
				continue;

			FileEntry file;
			file.fullPath = entry.path();
			file.relativePath = std::filesystem::relative(entry.path(), root);
			file.size = entry.file_size();
			file.lastWriteTime = entry.last_write_time().time_since_epoch().count();

			result.push_back(file);
		}

		return result;
	}

	std::vector<unsigned char> FileSystem::ReadFileBinary(const std::filesystem::path& path)
	{
		std::ifstream in(path, std::ios::ate | std::ios::binary);
		if (!in.is_open())
			return {};

		const std::streamsize size = in.tellg();
		in.seekg(0, std::ios::beg);
		if (size == 0)
			return {};

		std::vector<unsigned char> buffer(size);
		in.read(reinterpret_cast<char*>(buffer.data()), size);

		return buffer;
	}
}