#pragma once

#include "Plugin.h"
#include <unordered_map>

#include "EclipsedEngine.Core.hpp"

namespace Eclipse
{
	class ECL_API PluginManager final
	{
	public:
		static Plugin* CompileAndLoad(const char* source);
		static void Unload(const char* pluginName);

	private:
		static inline std::unordered_map<std::string, Plugin*> loadedplugins;
	};
}