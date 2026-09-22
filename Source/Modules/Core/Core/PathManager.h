#pragma once

#include <filesystem>
#include "Core.Core.hpp"

namespace Eclipse
{
	class CORE_API PathManager
	{
	public:
		static void Init();
		static void SetProjectPath(const char* projectPath);

		static bool ProjectSet();

	public:
		// The root to the project, this path contains the project and the binary builds. As well as cmake.
		static std::filesystem::path GetProjectRoot() { return projectRoot; }
		static std::filesystem::path GetEngineRoot() { return engineSourceRoot; }

		static std::filesystem::path GetEditorPath() { return GetProjectRoot() / "Editor"; }
		static std::filesystem::path GetAssetsPath() { return GetProjectRoot() / "Content"; }

		// Fix this, the directory does not exist if published a build.
		static std::filesystem::path GetEngineAssetsPath() { return GetEngineRoot().parent_path() / "Assets"; }
		
		static std::filesystem::path GetProjectLocalPath() { return GetProjectRoot() / "Temp"; }
		static std::filesystem::path GetArtifactsPath() { return GetProjectLocalPath() / "Artifacts"; }
		static std::filesystem::path GetBuildPath() { return GetProjectRoot() / "Bin"; }

		static std::filesystem::path GetSettingsPath() { return GetProjectRoot() / "Settings"; }

	private:
		static inline std::filesystem::path engineSourceRoot;

		static inline std::filesystem::path projectRoot;
	};
}