#pragma once

#include <string>
#include "Reflection.Core.hpp"

namespace Eclipse::Reflection
{
	class REFLECTION_API ReflectionFileScanner final 
	{
	public:
		static void ScanAndReflectFiles(const char* root);
		static void ReflectFile(const char* path);

	private:
		static std::string ReadFile(const char* path);
	};
}