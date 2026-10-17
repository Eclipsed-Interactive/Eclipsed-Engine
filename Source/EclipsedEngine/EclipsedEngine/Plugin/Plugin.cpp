#include "Plugin.h"

#include "Core/BatchScript.h"
#include "Core/PathManager.h"

#include <fstream>
#include <iostream>
#include <Windows.h>

#include "Core/Standard/System/Process.h"
#include "Core/Standard/Core/Asserts.hpp"

namespace Eclipse
{
	Plugin::Plugin(const char* source)
	{
		std::string path = source;
		
		std::ifstream pluginFile(path + "/.plugin");
		std::string pluginName;
		pluginFile >> pluginName;

		name = pluginName.substr(pluginName.find('=') + 1);

		const std::filesystem::path toolsPath = std::filesystem::path(path) / "Tools";

		if (!Process::RunBatchAndWait(toolsPath / "generate.bat", true))
			return;

		if (!Process::RunBatchAndWait(toolsPath / "compile.bat", true))
			return;


		std::string dllPath = "";
		std::string binPath = (std::filesystem::path(source) / "Bin").generic_string();
		std::filesystem::create_directories(binPath);
		for (auto i : std::filesystem::recursive_directory_iterator(binPath))
		{
			if (i.path().extension() == ".dll")
			{
				dllPath = i.path().generic_string();
				continue;
			}
		}

		dllModule = LoadLibraryA(dllPath.c_str());
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

};