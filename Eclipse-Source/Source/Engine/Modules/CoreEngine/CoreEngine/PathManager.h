#pragma once

#include <filesystem>

namespace Eclipse
{
	class PathManager
	{
	public:
		static void Init(const std::string& projectPath);

		static bool ProjectSet();

	public:
		// The root to the project, this path contains the project and the binary builds. As well as cmake.
		static std::filesystem::path GetProjectRoot() { return projectRoot; }
		static std::filesystem::path GetEngineRoot() { return engineSourceRoot; }

		static std::filesystem::path GetProjectPath() { return GetProjectRoot() / "Project"; }
		
		static std::filesystem::path GetEditorPath() { return GetProjectPath() / "Editor"; }
		static std::filesystem::path GetAssetsPath() { return GetProjectPath() / "Assets"; }
		static std::filesystem::path GetEngineAssetsPath() { return GetAssetsPath() / "EngineAssets"; }
		static std::filesystem::path GetProjectLocalPath() { return GetProjectPath() / "Local"; }
		static std::filesystem::path GetArtifactsPath() { return GetProjectLocalPath() / "Artifacts"; }
		static std::filesystem::path GetGameDllBuildPath() { return GetProjectPath() / "Local/Build"; }

		static std::filesystem::path GetSettingsPath() { return GetProjectPath() / "Settings"; }

	private:
		static inline std::filesystem::path engineSourceRoot;

		static inline std::filesystem::path projectRoot;
	};
}