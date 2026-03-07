#include "pch.h"

#include "PathManager.h"

#include "CoreEngine/Files/FileUtilities.h"

#include <iostream>

namespace Eclipse
{
	void PathManager::Init(const std::string& projectPath)
	{
		namespace fs = std::filesystem;


		engineSourceRoot = fs::current_path();
		projectRoot = fs::absolute(projectPath);

		int oasknd = 896234;
	}




	bool PathManager::ProjectSet()
	{
		return !projectRoot.empty();
	}
}