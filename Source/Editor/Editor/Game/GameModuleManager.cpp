#include "GameModuleManager.h"

#include "Core/BatchScript.h"
#include "Core/PathManager.h"
#include "EclipsedEngine/Plugin/PluginManager.h"

#include <Windows.h>

namespace Eclipse
{
	Plugin* GameModuleManager::game = nullptr;
	std::string GameModuleManager::name;
	std::string GameModuleManager::sourcepath;

	bool GameModuleManager::Load(const char* source)
	{
		sourcepath = source;
		game = PluginManager::CompileAndLoad(sourcepath.c_str());
		name = game->GetName();

		if (!game->Succeded())
			return false;

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

