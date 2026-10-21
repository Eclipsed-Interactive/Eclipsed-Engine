#pragma once

#include <string>
#include <unordered_map>

#define REFLECTION_FLAG(FLAG)
#define SERIALIZABLE
#define COMPONENT

namespace Eclipse::Reflection
{
	enum class ReflectionFlags : unsigned int
	{
		None = 0,
		Component = 1 << 0,
		Serializable = 1 << 1,
	};
}