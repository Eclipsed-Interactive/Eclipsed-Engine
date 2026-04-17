#pragma once

#include <string>

namespace Eclipse::Utilities
{
	class File final
	{
	public:
		File() = default;
		File(const char* path, int ios);

		~File();

	public:
		static void Create(const char* dirPath, const char* fileName);

		void Write(const char* str, int size, int idx = 0);
		void WriteLine(const char* str, int size);

		void Read(char* outStr, int size, int idx = 0);
		std::string ReadLine();

	private:
		int ios = std::ios::binary | std::ios::in | std::ios::out;
		std::fstream stream;
		bool streamOpened = false;

		int readLineIdx = 0;
		int writeLineIdx = 0;
	};
}