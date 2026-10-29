#include "SceneManager.h"

#include "Assets/Core/AssetDatabase.h"
#include "Assets/AssetManager.h"
#include "Core/MainSingleton.h"

#include "SceneSerializer.h"
#include "Core/Settings/EditorSettings.h"

#include "Physics/PhysicsEngine.h"
#include "Renderer/RenderCommands/CommandList.h"

#include "ECS/ComponentManager.h"

namespace Eclipse
{
	std::string SceneManager::CaptureSnapshot()
	{
		return SceneSerializer::SaveScene();
	}

	void SceneManager::RestoreSnapshot(const std::string& snapshot)
	{
		Clear();

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

	void SceneManager::ReloadActive()
	{
		Assets::Scene scene = ActiveScene;

		UnloadActive();
		Load(scene);
	}

	void SceneManager::SaveActive()
	{
		const std::string serializedString = SceneSerializer::SaveScene(true);

		Assets::AssetDatabase& database = MainSingleton::GetInstance< Assets::AssetDatabase>();
		const Assets::AssetMeta& meta = database.GetProcessedFile(ActiveScene.GetAssetID());

		const std::filesystem::path path = meta.fullPath;
		Settings::EditorSettings::SetLastActiveScene(meta.guid.ToString());

		ActiveScene.dataPtr->sourceBlob = serializedString;

		std::ofstream file(path, std::ios::binary);
		if (!file)
		{
			throw std::runtime_error("The file failed to open.");
		}

		file << serializedString;
		file.close();
	}

	void SceneManager::LoadLastActive()
	{
		Assets::GUID id;
		const std::string& idString = Settings::EditorSettings::GetLastActiveScene();
		if (idString.empty())
			return;

		id.FromString(idString);

		Load(id);
	}

	void SceneManager::Load(const char* path)
	{
		std::filesystem::path fullpath = "";

		if (std::filesystem::exists(path))
		{
			fullpath = path;
		}
		else
		{
			fullpath = PathManager::GetAssetsPath() / path;
			fullpath.replace_extension(".scene");
		}

		UnloadActive();
		SetActiveSceneType(SceneType::World);

		Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
		const Assets::GUID guid = database.GetGUIDFromFullPath(fullpath);
		ActiveScene = Assets::AssetManager::Load<Assets::Scene>(guid);
		Settings::EditorSettings::SetLastActiveScene(guid.ToString());

		SceneSerializer::LoadScene(ActiveScene);

#ifdef ECL_EDITOR
		MarkClean();
#endif
	}

	void SceneManager::Load(Assets::Scene scene)
	{
		UnloadActive();
		SetActiveSceneType(SceneType::World);

		Settings::EditorSettings::SetLastActiveScene(scene.GetAssetID().ToString());

		ActiveScene = scene;
		SceneSerializer::LoadScene(ActiveScene);

#ifdef ECL_EDITOR
		MarkClean();
#endif
	}

	void SceneManager::Load(Assets::GUID guid)
	{
		try
		{
			Assets::Scene scene = Assets::AssetManager::Load<Assets::Scene>(guid);

			UnloadActive();
			SetActiveSceneType(SceneType::World);

			Settings::EditorSettings::SetLastActiveScene(guid.ToString());
			ActiveScene = scene;

			SceneSerializer::LoadScene(ActiveScene);

#ifdef ECL_EDITOR
			MarkClean();
#endif
		}
		catch(...)
		{
			// Scene dont exist.
		}
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

	Assets::Scene SceneManager::GetActive()
	{
		return ActiveScene;
	}
}
