#include "PathManager.h"

#include <iostream>
#include <cstdlib>
#include <Windows.h>

#include <Core/Logger/DebugLogger.h>

namespace Eclipse
{
	void PathManager::Init()
	{
		namespace fs = std::filesystem;

		wchar_t exePath[MAX_PATH];
		const DWORD len = GetModuleFileNameW(nullptr, exePath, MAX_PATH);
		if (len > 0 && len < MAX_PATH)
			engineSourceRoot = fs::path(exePath).parent_path();
		else
			engineSourceRoot = fs::current_path();

		std::string path = engineSourceRoot.parent_path().generic_string();
		_putenv_s("ecl_path", path.c_str());

		std::string outputPath = engineSourceRoot.generic_string();
		_putenv_s("ecl_output_path", outputPath.c_str());

		LOG("Set engine root path: " + engineSourceRoot.generic_string());
	}

	void PathManager::SetProjectPath(const char* projectPath)
	{
		namespace fs = std::filesystem;
		projectRoot = projectPath;

		LOG("Set project root path: " + projectRoot.generic_string());

	}

	bool PathManager::ProjectSet()
	{
		return !projectRoot.empty();
	}
}