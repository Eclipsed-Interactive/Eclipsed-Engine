#pragma once

#include <string>

namespace Eclipse::Editor
{
	class EditorProjectManager
	{
	public:
		static std::string LoadOrSelectProject();
		static std::string LoadProjectFromFile();
		static void SaveProjectPath(const std::string& path);
	};
}