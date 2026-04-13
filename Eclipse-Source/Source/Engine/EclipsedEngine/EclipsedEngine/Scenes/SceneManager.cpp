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
		
		SetActiveSceneType(Default);
		
		std::string path = nameOrPath;
		if (!std::filesystem::path(nameOrPath).has_extension()) {
			path += ".scene";
		}

		std::string fullPath = (PathManager::GetAssetsPath() / path).generic_string();
		if (std::filesystem::exists(path) || std::filesystem::exists(fullPath))
		{
			myActiveScene = nameOrPath;
			SceneLoader::Load(fullPath.c_str());
		}
		else
		{
			LOG_WARNING("Scene dont exist at asset root path. | " + nameOrPath);
		}
	}

	void SceneManager::LoadScene(unsigned idx)
	{
		if (myScenePaths.empty()) return;

		SceneLoader::Load((PathManager::GetAssetsPath() / myScenePaths[idx]).generic_string().c_str());

		myActiveScene = std::filesystem::path(myScenePaths[idx]).filename().stem().string();
	}

	void SceneManager::UnloadScene()
	{
		SceneLoader::UnloadScene();
	}

	void SceneManager::ReloadActiveScene()
	{
		LoadScene(myActiveScene);
	}

	void SceneManager::SaveScenes()
	{
	}

	void SceneManager::SaveActiveScene()
	{
		if (myActiveScene.empty()) return;
		
		SceneLoader::Save(myActiveScene.c_str());
	}

	void SceneManager::AddScene(const std::string& aPath)
	{
		std::filesystem::path path = std::filesystem::relative(aPath, PathManager::GetAssetsPath());

		std::string name = path.filename().stem().string();
		myNameToIdx[name] = (unsigned)myScenePaths.size();
		myScenePaths.push_back(path.generic_string());
	}

	void SceneManager::LoadSceneData()
	{
		using namespace rapidjson;

		const std::vector<std::string>& sceneIndex = Settings::BuildSettings::GetSceneIndex();
		if (sceneIndex.empty()) return;

		for (int i = 0; i < sceneIndex.size(); i++)
		{
			myNameToIdx[std::filesystem::path(sceneIndex[i]).filename().stem().generic_string()] = i;
			myScenePaths.push_back(sceneIndex[i]);
		}
	}

	void SceneManager::SaveSceneData()
	{
		Settings::BuildSettings::SetSceneIndex(myScenePaths);
		Settings::BuildSettings::Save();
	}

	void SceneManager::ClearScene()
	{
		myActiveScene = "";

		CommandListManager::ResetAllCommandLists();

		ComponentManager::Clear();
		Reflection::ReflectionManager::ClearList();

		PhysicsEngine::CleanUp();
	}

	std::unordered_map<std::string, unsigned>& SceneManager::GetNameToIdx() { return myNameToIdx; }
	std::vector<std::string>& SceneManager::GetScenePaths() { return myScenePaths; }
	const char* SceneManager::GetActiveScene() 
	{ 
		return myActiveScene.c_str(); 
	}

	void SceneManager::SetActiveScene(const char* anActiveScene)
	{
		myActiveScene = anActiveScene;
	}

	SceneManager::SceneType SceneManager::GetActiveSceneType()
	{
		return myActiveSceneType;
	}

	void SceneManager::SetActiveSceneType(SceneType aType)
	{
		myActiveSceneType = aType;
	}
}
