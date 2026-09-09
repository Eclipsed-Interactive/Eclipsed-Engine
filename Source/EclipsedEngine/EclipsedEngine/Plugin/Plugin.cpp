#include "Plugin.h"

#include "Core/BatchScript.h"
#include "Core/PathManager.h"

#include <fstream>
#include <iostream>
#include <Windows.h>

namespace Eclipse
{
	Plugin::Plugin(const char* source)
	{
		std::string path = source;

		std::ifstream pluginFile(path + "/.plugin");
		std::string pluginName;
		pluginFile >> pluginName;

		name = pluginName.substr(pluginName.find('=') + 1);

		BatchScript generateScript(
			path + "/Tools",
			"generate.bat"
		);

		BatchScript buildScript(
			path + "/Tools",
			"compile.bat"
		);

		generateScript.Run();
		buildScript.Run();

		std::string binPath = (PathManager::GetEngineRoot() / (name + ".dll")).generic_string();
		dllModule = LoadLibraryA(binPath.c_str());
		if (dllModule)
		{
			initFunc = GetMethod<Func>("Init");
		}
	}

	Func Plugin::GetInitFunc() const
	{
		return initFunc;
	}

	bool Plugin::Succeded() const
	{
		return succeded;
	}

	const char* Plugin::GetName() const
	{
		return name.c_str();
	}

}