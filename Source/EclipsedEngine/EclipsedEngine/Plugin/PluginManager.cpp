#include "PluginManager.h"

#include "Core/BatchScript.h"
#include "Core/PathManager.h"

#include <fstream>
#include <iostream>
#include <Windows.h>

namespace Eclipse
{
	Plugin* PluginManager::CompileAndLoad(const char* source)
	{
		Plugin* plug = new Plugin(source);
		if (!plug->Succeded())
		{
			delete plug;
			return nullptr;
		}

		loadedplugins[plug->GetName()] = plug;
		plug->GetInitFunc()();
	}

	void PluginManager::Unload(const char* pluginName)
	{
		auto it = loadedplugins.find(std::string(pluginName));

		if (it == loadedplugins.end())
			return;

		Plugin* plug = it->second;

		delete plug;
		loadedplugins.erase(it);
	}
}