#include "SceneManager.h"

#include "AssetSystem/Assets/Types/Scene.h"
#include "Core/MainSingleton.h"

#include "SceneSerializer.h"
#include "Core/PathManager.h"

#include "Physics/PhysicsEngine.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "ECS/ComponentManager.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

namespace Eclipse
{
	std::string SceneManager::CaptureSnapshot()
	{
		return SceneSerializer::SaveScene();
	}

	void SceneManager::RestoreSnapshot(const std::string& snapshot)
	{
		Clear();

		PhysicsEngine::InitWorld();

		SceneSerializer::LoadScene(snapshot);
	}

	void SceneManager::UnloadActive()
	{
		ActiveScene = {};

		Clear();
	}

	void SceneManager::Clear()
	{
		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
		commandListManager->ResetAllCommandLists();

		ComponentManager::Clear();

		PhysicsEngine::CleanUp();
	}

	void SceneManager::SetOnSceneChangeCallback(std::function<void(GUID guid)> callBack)
	{
		onSceneChangeCallback = callBack;
	}

	void SceneManager::ReloadActive()
	{
		Scene scene = ActiveScene;

		UnloadActive();
		Load(scene);
	}

	void SceneManager::SaveActive()
	{
		//using namespace rapidjson;

		//if (!ActiveScene.IsReady())
		//	return;

		//const SerializedScene scene = SceneSerializer::Serialize(ActiveScene.GetGUID(), true);
	
		//const std::filesystem::path path = meta.fullPath;
		//onSceneChangeCallback(meta.guid);



		//ActiveScene.dataPtr->sourceBlob = scene.manifest;

		//std::filesystem::create_directories(PathManager::GetAssetsPath() / "__external__" / "Scenes" / meta.fileName);

		//for (auto& obj : scene.objects)
		//{
		//	std::ofstream sceneObj(PathManager::GetAssetsPath() / "__external__" / "Scenes" / meta.fileName / obj.guid.ToString(), std::ios::binary);
		//	sceneObj << obj.json;
		//}


		//std::ofstream file(path, std::ios::binary);
		//if (!file)
		//{
		//	throw std::runtime_error("The file failed to open.");
		//}

		//file << scene.manifest;
		//file.close();
	}


	void SceneManager::Load(const char* path)
	{
//		std::filesystem::path fullpath = "";
//
//		if (std::filesystem::exists(path))
//		{
//			fullpath = path;
//		}
//		else
//		{
//			fullpath = PathManager::GetAssetsPath() / path;
//			fullpath.replace_extension(".scene");
//		}
//
//		UnloadActive();
//		SetActiveSceneType(SceneType::World);
//		PhysicsEngine::InitWorld();
//
//		Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
//		const GUID guid = database.GetGUIDFromFullPath(fullpath);
//		ActiveScene = Assets::AssetManager::Load<Assets::Scene>(guid);
//		onSceneChangeCallback(guid);
//
//		SceneSerializer::LoadScene(ActiveScene);
//
//#ifdef ECL_EDITOR
//		MarkClean();
//#endif
	}

	void SceneManager::Load(Scene scene)
	{

//		UnloadActive();
//		SetActiveSceneType(SceneType::World);
//		PhysicsEngine::InitWorld();
//
//		onSceneChangeCallback(scene.GetAssetID());
//
//		ActiveScene = scene;
//		SceneSerializer::LoadScene(ActiveScene);
//
//#ifdef ECL_EDITOR
//		MarkClean();
//#endif
	}

	void SceneManager::Load(GUID guid)
	{
//		try
//		{
//			Assets::Scene scene = Assets::AssetManager::Load<Assets::Scene>(guid);
//
//			UnloadActive();
//			SetActiveSceneType(SceneType::World);
//			PhysicsEngine::InitWorld();
//
//			onSceneChangeCallback(guid);
//			ActiveScene = scene;
//
//			SceneSerializer::LoadScene(ActiveScene);
//
//#ifdef ECL_EDITOR
//			MarkClean();
//#endif
//		}
//		catch(...)
//		{
//			// Scene dont exist.
//		}
	}

	void SceneManager::SetActiveSceneType(SceneType type)
	{
		ActiveSceneType = type;
	}

	SceneManager::SceneType SceneManager::GetActiveSceneType()
	{
		return ActiveSceneType;
	}

	unsigned SceneManager::GetActivePrefab()
	{
		return ActivePrefabEditSceneID;
	}

	Scene SceneManager::GetActive()
	{
		return ActiveScene;
	}
}
