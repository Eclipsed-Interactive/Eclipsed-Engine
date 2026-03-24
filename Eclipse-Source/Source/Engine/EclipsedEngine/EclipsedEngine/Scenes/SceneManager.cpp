#include "SceneManager.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "SceneLoader.h"

#include "CoreEngine/PathManager.h"

#include "PhysicsEngine/PhysicsEngine.h"
#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "EntityEngine/ComponentManager.h"
#include "CoreEngine/Settings/BuildSettings.h"
#include "CoreEngine/Settings/EditorSettings.h"

#include <fstream>
//#include "CoreEngine/Settings/.h"
#include "CoreEngine/Debug/DebugLogger.h"

namespace Eclipse
{
	void SceneManager::LoadScene(const std::string& nameOrPath)
	{
		if (nameOrPath.empty()) return;

		std::string path = nameOrPath;
		if (!std::filesystem::path(nameOrPath).has_extension()) {
			path += ".scene";
		}

		std::string fullPath = (PathManager::GetAssetsPath() / path).generic_string();
		if (std::filesystem::exists(path) || std::filesystem::exists(fullPath))
		{
			ActiveScene = nameOrPath;
			SceneLoader::Load(fullPath.c_str());
		}
		else
		{
			LOG_WARNING("Scene dont exist at asset root path. | " + nameOrPath);
		}
	}

	void SceneManager::LoadScene(unsigned idx)
	{
		if (scenePaths.empty()) return;

		SceneLoader::Load((PathManager::GetAssetsPath() / scenePaths[idx]).generic_string().c_str());

		ActiveScene = std::filesystem::path(scenePaths[idx]).filename().stem().string();
	}

	void SceneManager::ReloadActiveScene()
	{
		LoadScene(ActiveScene);
	}

	void SceneManager::SaveScenes()
	{
	}

	void SceneManager::SaveActiveScene()
	{
		if (ActiveScene.empty()) return;
		
		SceneLoader::Save(ActiveScene.c_str());
	}

	void SceneManager::AddScene(const std::string& aPath)
	{
		std::filesystem::path path = std::filesystem::relative(aPath, PathManager::GetAssetsPath());

		std::string name = path.filename().stem().string();
		nameToIdx[name] = (unsigned)scenePaths.size();
		scenePaths.push_back(path.generic_string());
	}

	void SceneManager::LoadSceneData()
	{
		using namespace rapidjson;

		std::cout << std::filesystem::current_path() << std::endl;
		const std::vector<std::string>& sceneIndex = Settings::BuildSettings::GetSceneIndex();
		if (sceneIndex.empty()) return;

		for (int i = 0; i < sceneIndex.size(); i++)
		{
			nameToIdx[std::filesystem::path(sceneIndex[i]).filename().stem().generic_string()] = i;
			scenePaths.push_back(sceneIndex[i]);
		}
	}

	void SceneManager::SaveSceneData()
	{
		Settings::BuildSettings::SetSceneIndex(scenePaths);
		Settings::BuildSettings::Save();
	}

	void SceneManager::ClearScene()
	{
		ActiveScene = "";

		CommandListManager::ResetAllCommandLists();

		ComponentManager::Clear();
		Reflection::ReflectionManager::ClearList();

		PhysicsEngine::CleanUp();
	}

	std::unordered_map<std::string, unsigned>& SceneManager::GetNameToIdx() { return nameToIdx; }
	std::vector<std::string>& SceneManager::GetScenePaths() { return scenePaths; }
	const char* SceneManager::GetActiveScene() 
	{ 
		return ActiveScene.c_str(); 
	}
}