#include "PathManager.h"

#include <iostream>
#include <cstdlib>

namespace Eclipse
{
	void PathManager::Init()
	{
		namespace fs = std::filesystem;

		engineSourceRoot = fs::current_path();

		std::string path = engineSourceRoot.parent_path().generic_string();
		_putenv_s("ecl_path", path.c_str());

		std::string outputPath = engineSourceRoot.generic_string();
		_putenv_s("ecl_output_path", outputPath.c_str());
	}

	void PathManager::SetProjectPath(const char* projectPath)
	{
		namespace fs = std::filesystem;
		projectRoot = projectPath;
	}

	bool PathManager::ProjectSet()
	{
		return !projectRoot.empty();
	}
}