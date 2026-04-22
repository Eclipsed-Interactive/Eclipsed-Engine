#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"

#include "CoreEngine/Files/DirectoryTree.h"
#include "EclipsedEngine/Editor/Windows/WindowTypes/AssetWindow/AssetWindowContextMenu.h"
#include "CoreEngine/Files/FileWatcher.h"

namespace Eclipse::Editor
{
	class AssetWindow final : public AbstractWindow
	{
		EDITOR_WINDOW_BASE_2(AssetWindow, "Assets")

	public:
		void Open();

		void Update();
		void LoadAssets();

	private:

		void DrawAssetView();
		void DrawAssetHierachy();
		void DrawAssetHierachyEntry(Utilities::FileNode* node);


		void OpenFile(const Utilities::FileInfo& fifo);
		void OnFileAdded(const Editor::FileWatcherEvent& e);

	private:
		bool CheckFileDoubleClicked();
		bool CheckFileClicked();

		void DrawAssetViewBreadcrumb();

		void DrawAssetViewEntry(const Utilities::FileNode* parent);
		void CheckAssetViewEntryClicked(const Utilities::FileNode* parent);

	private:
		static inline float folderStructureWidth = 215.f;
		static inline float scrollBarWidth = 15.f;

		float myButtonSizeMultiplier = 1.f;
		unsigned entryIndex = 0;
		static inline int ActiveEntryIndex = -1;

	private:
		Utilities::DirectoryTree dirTree;

		Utilities::FileNode* Active_View_Node;
		Utilities::FileNode* Active_Hierarchy_Node;

		AssetWindowContextMenu ctxMenu{};

		bool shouldReloadAssets = 0;

	public:
		static inline std::filesystem::path ActivePath;
	};
}