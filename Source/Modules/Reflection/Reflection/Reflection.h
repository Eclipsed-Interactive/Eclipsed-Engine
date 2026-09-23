#pragma once

#include "Reflection.Core.hpp"
#include <filesystem>

namespace Eclipse::Reflection
{
	void REFLECTION_API Reflect(const std::filesystem::path& path);
}