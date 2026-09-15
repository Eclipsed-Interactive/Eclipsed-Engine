#include "Core/Files/MemoryMapped.h"

#include <Windows.h>
#include <filesystem>

namespace Eclipse
{
	constexpr uint64_t DEFAULT_SIZE = 100ull * 1024ull * 1024ull;

	MemoryMappedFile::MemoryMappedFile(const char* path)
	{
		Open(path);
	}

	MemoryMappedFile::~MemoryMappedFile()
	{
		FlushViewOfFile(data, 0);
		FlushFileBuffers(fileHandle);
	}

	void MemoryMappedFile::Open(const char* path)
	{
		size = DEFAULT_SIZE;

		if (std::filesystem::exists(path))
		{
			auto existingSize = std::filesystem::file_size(path);

			if (existingSize > 0)
				size = existingSize;
		}

		HANDLE file = CreateFileA(
			path,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (file == INVALID_HANDLE_VALUE)
			return;

		LARGE_INTEGER liSize;
		liSize.QuadPart = size;

		if (!SetFilePointerEx(file, liSize, nullptr, FILE_BEGIN))
		{
			CloseHandle(file);
			return;
		}

		if (!SetEndOfFile(file))
		{
			CloseHandle(file);
			return;
		}

		HANDLE mappedFile = CreateFileMappingA(
			file,
			nullptr,
			PAGE_READWRITE,
			liSize.HighPart,
			liSize.LowPart,
			nullptr
		);

		if (!mappedFile)
		{
			CloseHandle(file);
			return;
		}

		data = MapViewOfFile(
			mappedFile,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			size
		);

		if (!data)
		{
			CloseHandle(mappedFile);
			CloseHandle(file);
			return;
		}

		fileHandle = file;
		mappedHandle = mappedFile;
	}
}