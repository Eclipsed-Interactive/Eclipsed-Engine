#include "pch.h"

#include "File.h"

#include <filesystem>
#include <fstream>

namespace Eclipse::Utilities
{
	File::File(const char* path, int ios)
	{
		std::filesystem::path fullPath = path;

		std::string fileName = fullPath.filename().string();
		std::string dirPath = fullPath.parent_path().generic_string();

		File::Create(dirPath.c_str(), fileName.c_str());

		stream = std::fstream(path, ios);
		if (stream.is_open()) streamOpened = true;
	}

	Eclipse::Utilities::File::~File()
	{
		stream.close();
		streamOpened = false;
	}

	void File::Create(const char* dirPath, const char* fileName)
	{
		std::filesystem::create_directories(dirPath);
		std::ofstream file(fileName);
		file.close();
	}

	void File::Write(const char* str, int size, int idx)
	{
		if (str == nullptr) return;

		stream.seekp(idx);
		stream.write(str, size);
	}

	void File::WriteLine(const char* str, int size)
	{
		stream.seekp(writeLineIdx);
		stream.write(str, size);
		stream.write("\n", 1);
		writeLineIdx = stream.tellp();
	}

	void File::Read(char* outStr, int size, int idx)
	{
		if (outStr == nullptr) return;

		stream.seekg(idx);
		stream.read(outStr, size);
	}

	std::string File::ReadLine()
	{
		return "";
	}
}