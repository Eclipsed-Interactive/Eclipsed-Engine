#include "Reflection.h"

#include "Core/PathManager.h"
#include "ReflectionFileScanner.h"

#include "TypeRegistry.h"
#include "ReflectionGenerator.h"

namespace Eclipse::Reflection
{
	void Reflect(const std::filesystem::path& path)
	{
		ReflectionFileScanner::ScanAndReflectFiles(path);

		ReflectionGenerator::Clear(path);
		ReflectionGenerator::Generate(path);
	}
} 