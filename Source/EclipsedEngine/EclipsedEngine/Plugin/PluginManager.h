#pragma once

#include "Plugin.h"
#include <unordered_map>

namespace Eclipse
{
	class PluginManager final
	{
	public:
		static void Load();
		static void LoadAndCompile(const char* source);

		static void Compile(const char* source);

	private:
		static inline std::unordered_map<std::string, Plugin*> loadedplugins;
	};
}