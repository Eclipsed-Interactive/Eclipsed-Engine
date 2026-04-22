#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace Eclipse
{
	class SceneManager
	{
	public:
		static void LoadScene(const std::string& nameOrPath);
		static void LoadScene(unsigned idx);

		static void UnloadScene();

		static void ReloadActiveScene();

		static void SaveScenes();
		static void SaveActiveScene();

		static void AddScene(const std::string& aPath);

		static void LoadSceneData();
		static void SaveSceneData();

		static void ClearScene();

		static std::unordered_map<std::string, unsigned>& GetNameToIdx();
		static std::vector<std::string>& GetScenePaths();
		static const char* GetActiveScene();

		
		enum SceneType
		{
			Default,
			Prefab
		};

		static void SetActiveSceneType(SceneType aType);
		static SceneType GetActiveSceneType();
		static void SetActiveScene(const char* anActiveScene);
		static inline unsigned ActivePrefabEditSceneID;
		
	private:
		static inline SceneType myActiveSceneType = Default;
		
		static inline std::string myActiveScene;

	private:
		static inline std::unordered_map<std::string, unsigned> myNameToIdx;
		static inline std::vector<std::string> myScenePaths;
	};
}