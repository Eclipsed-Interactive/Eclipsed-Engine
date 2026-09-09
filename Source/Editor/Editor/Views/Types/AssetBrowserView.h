#pragma once

#include "Editor/Views/IEditorView.h"

#include "Core/Files/DirectoryTree.h"
#include "Core/Files/FileWatcher.h"

#include "AssetBrowserViewContextMenu.h"

namespace Eclipse::Editor
{
	class AssetBrowserView : public EditorView<AssetBrowserView>
	{
		BASIC_VIEW("Asset Browser")

	public:
		void OnOpen() override;
		void Draw() override;

	private:
		void DrawAssetView();
		void DrawAssetHierachy();
		void DrawAssetHierachyEntry(const Utilities::FileNode* node);


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
		Utilities::DirectoryTree engineTree;

		Utilities::FileNode const* Active_View_Node;
		Utilities::FileNode const* Active_Hierarchy_Node;

		AssetBrowserViewContextMenu ctxMenu{};

		bool shouldReloadAssets = 0;

	public:
		static inline std::function<unsigned(char* data)> CreateGameobjectFunc;
		static inline std::function<void()> InitNewPhysicsScene;

		static inline std::filesystem::path ActivePath;
	};
}