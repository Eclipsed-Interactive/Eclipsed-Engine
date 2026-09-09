#ifdef ECL_EDITOR
#pragma once

#include "Editor/Views/AbstractContextMenu.h"
#include <filesystem>
namespace Eclipse::Editor 
{
	class AssetBrowserViewContextMenu  : public AbstractContextMenu {
	public:
		AssetBrowserViewContextMenu();

		void SetActivePath(const std::filesystem::path& aPath);
		const std::filesystem::path& GetActivePath();
		
		void Update() override;
		void UpdateAlways() override;

	private:
		void CreateMenu();
		std::filesystem::path activePath;


		std::filesystem::path activePathAtRenaming;
		bool renameModal = false;
		bool focusNameRename = true;
		char tempName[512];
	};
}
#endif