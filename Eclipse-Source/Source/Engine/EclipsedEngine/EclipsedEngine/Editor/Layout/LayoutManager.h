#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <filesystem>
#include <functional>

#include "CoreEngine/Settings/SettingsModels/EditorLayout.h"

namespace Eclipse::Editor
{ 
	struct LayoutWindowData {
		int id;
		std::string name;
	};

	class LayoutManager
	{
	public:
		static std::vector<LayoutWindowData> OpenLayout(const std::string_view& layoutName);

		static void ImportLayout();
		static void SaveLayout();
		static void SaveAsNewLayout(std::function<void()> callback);
		static void SaveLayout(const std::string& name);
		static void NewLayout();

		static void Update();

		static void LoadLayouts();

		static const std::vector<std::string>& GetLayouts();

		static std::string GetActiveLayoutName();
		static std::string GetNewLayoutName();

		static void SaveActiveLayout();

	private:
		static std::filesystem::path GetLayoutPath(const std::string& filename);

	private:
		static std::string myActiveLayout;
		
		static bool myOpenNameSelectPopup;
		static char myNameSelectBuffer[128];

		static std::vector<std::string> myLayouts;

		static std::function<void()> onSaveCallback;
	};
}