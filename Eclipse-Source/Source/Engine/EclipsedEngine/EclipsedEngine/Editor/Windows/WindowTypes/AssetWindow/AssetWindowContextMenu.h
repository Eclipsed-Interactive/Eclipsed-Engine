#pragma once

#include "EclipsedEngine/Editor/Windows/AbstractContextMenu.h"

namespace Eclipse::Editor 
{
	class AssetWindowContextMenu  : public AbstractContextMenu {
	public:
		AssetWindowContextMenu();

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