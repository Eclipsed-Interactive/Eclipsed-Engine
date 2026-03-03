#include "GameCompiler.h"

#include "CoreEngine/PathManager.h"
#include <filesystem>

namespace Eclipse
{
	void GameCompiler::CompileGame()
	{
		GenerateProject();
		Compile();
		GenerateForcelink();
	}

	void GameCompiler::GenerateForcelink()
	{
		const std::filesystem::path& projectRoot = PathManager::GetProjectRoot();
		const std::filesystem::path& forcelink = projectRoot / "Forcelink.cpp";
		const std::filesystem::path& source = projectRoot / "Source";
	}

	void GameCompiler::GenerateProject()
	{
		std::string sourceDir = (PathManager::GetEngineRoot().parent_path().parent_path() / "Tools/").generic_string();
		std::string buildCmakeCommand = "cd /d \"" + sourceDir + "\" && generate-game-editor.bat \""
			+ PathManager::GetProjectRoot().generic_string() + "\" \""
			+ PathManager::GetEngineRoot().parent_path().generic_string() + "\"";
		std::system(buildCmakeCommand.c_str());
	}

	void GameCompiler::Compile()
	{
		std::string buildDir = (PathManager::GetProjectRoot()).generic_string();
		std::string buildDLLCommand = "cd /d \"" + buildDir + "\" && cmake --build Library/Engine-Build";
		std::system(buildDLLCommand.c_str());
	}
}