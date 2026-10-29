#pragma once

#include "EclipsedEngine.Core.hpp"

#include "Assets/Assets/SceneAsset.h"

namespace Eclipse
{
	class ECL_API SceneManager
	{
	public:
		static void UnloadActive();
		static void ReloadActive();

		static void Load(const char* path);
		static void Load(Assets::Scene scene);
		static void Load(Assets::GUID guid);

		static void SaveActive();

		static void LoadLastActive();

		static void Clear();

	public:
		static std::string CaptureSnapshot();
		static void RestoreSnapshot(const std::string& snapshot);

	public:
		enum SceneType
		{
			World,
			Prefab
		};

		static void SetActiveSceneType(SceneType aType);
		static SceneType GetActiveSceneType();

		static unsigned GetActivePrefab();
		static Assets::Scene GetActive();

	private:
		static inline Assets::Scene ActiveScene;

		static inline unsigned ActivePrefabEditSceneID;

		static inline SceneType ActiveSceneType = SceneType::World;

	private:
#ifdef ECL_EDITOR
		static inline bool _IsDirty = false;
		static inline size_t _SceneHash = 0;

	public:
		static void MarkDirty() { _IsDirty = true; }
		static void MarkClean() { _IsDirty = false; _SceneHash = HashActiveScene(); }

		static size_t HashActiveScene() { return std::hash<std::string>{}(CaptureSnapshot()); }
		static void CheckDirty()
		{
			if (_SceneHash == 0)
				MarkClean();

			if (_SceneHash != HashActiveScene())
				MarkDirty();
		}

		static bool IsDirty() { return _IsDirty; }
#endif
	};
}