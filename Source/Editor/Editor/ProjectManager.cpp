#include "ProjectManager.h"

#include "Core/PathManager.h"
#include "Core/Files/FileUtilities.h"
#include <fstream>
#include <filesystem>

namespace Eclipse::Editor
{
	std::string ProjectManager::LoadOrSelectProject()
	{
		namespace fs = std::filesystem;

		std::string path;
		if (fs::exists(".ini"))
		{
			path = LoadProjectFromFile();
		}

		if (path.empty())
		{
			path = Files::SelectFolderDialog();
		}


		SaveProjectPath(path);

		PathManager::SetProjectPath(path.c_str());

		return path;
	}

	std::string ProjectManager::LoadProjectFromFile()
	{
		std::ifstream in(".ini");
		std::string path;
		std::getline(in, path);

		in.close();
		return path;
	}

	void ProjectManager::SaveProjectPath(const std::string& path)
	{
		std::ofstream out(".ini", std::ios::trunc);
		out.write(path.c_str(), path.size());
		out.close();
	}
}