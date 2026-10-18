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
		namespace fs = std::filesystem;

		std::string path = source;

		std::ifstream pluginFile(path + "/.plugin");
		std::string pluginName;
		pluginFile >> pluginName;

		name = pluginName.substr(pluginName.find('=') + 1);


		fs::path sourcePath = source;
		fs::path dllPath = "";

		for (auto& entry : fs::recursive_directory_iterator(sourcePath / "Bin"))
		{
			if (entry.path().extension().generic_string() == ".dll")
			{
				dllPath = entry.path();
				break;
			}
		}


		fs::file_time_type dllLastWriteTime = {};

		if (fs::exists(dllPath))
			dllLastWriteTime = fs::last_write_time(dllPath);

		bool wasChanged = false;
		for (auto& entry : fs::recursive_directory_iterator(sourcePath / "Content"))
		{
			if (!entry.is_regular_file())
				continue;

			if (fs::last_write_time(entry.path()) > dllLastWriteTime)
			{
				wasChanged = true;
			}
		}

		const std::filesystem::path toolsPath = std::filesystem::path(path) / "Tools";

		if (wasChanged)
		{
			if (!Process::RunBatchAndWait(toolsPath / "generate.bat", true))
				return;

			if (!Process::RunBatchAndWait(toolsPath / "compile.bat", true))
				return;
		}


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

		fs::path newDllPath = PathManager::GetEngineRoot() / (name + ".dll");
		fs::copy_file(dllPath, newDllPath);

		const std::string dllString = newDllPath.generic_string();
		dllModule = LoadLibraryA(dllString.c_str());
		if (dllModule)
		{
			initFunc = GetMethod<Func>("Init");
		}
	}

	Plugin::~Plugin()
	{
		if (dllModule)
		{
			FreeLibrary((HMODULE)dllModule);
			dllModule = nullptr;

			initFunc = nullptr;
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