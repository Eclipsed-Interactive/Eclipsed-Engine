#include "GameModuleManager.h"

#include "Core/BatchScript.h"
#include "Core/PathManager.h"
#include "EclipsedEngine/Plugin/PluginManager.h"

#include <Windows.h>
#include "Core/Standard/System/Process.h"

namespace Eclipse
{
	Plugin* GameModuleManager::game = nullptr;
	std::string GameModuleManager::name;
	std::string GameModuleManager::sourcepath;

	bool GameModuleManager::Load(const char* source)
	{
		const std::string exePath = (PathManager::GetEngineRoot() / "HeaderTool.exe").generic_string();

		const std::string contentPath = (PathManager::GetProjectRoot() / "Content").generic_string();
		const std::string outputPath = (PathManager::GetProjectRoot() / "Generated" / "Reflection").generic_string();

		const char* args[] = {
			contentPath.c_str(),
			outputPath.c_str()
		};

		const bool success = Process::RunAndWait(exePath.c_str(), args, 2);

		sourcepath = source;
		game = PluginManager::CompileAndLoad(sourcepath.c_str());
		//name = game->GetName();

		if (!game->Succeded())
			return false;

		auto registerTypes = game->GetMethod("RegisterReflectedTypes");
		if (registerTypes)
			registerTypes();

		return true;
	}

	void GameModuleManager::Unload()
	{
		if (game)
			PluginManager::Unload(game->GetName());
	}

	bool GameModuleManager::Reload()
	{
		if (!game) return false;

		PluginManager::Unload(name.c_str());
		game = PluginManager::CompileAndLoad(sourcepath.c_str());

		return game->Succeded();
	}

	bool GameModuleManager::IsLoaded()
	{
		return game != nullptr;
	}

}

