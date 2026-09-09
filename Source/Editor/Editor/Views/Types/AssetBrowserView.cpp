#include "AssetBrowserView.h"

#include "ImGui/imgui.h"

#include "Core/MainSingleton.h"
#include "Assets/Core/AssetDatabase.h"

#include "Editor/Common/TextureIconManager.h"

#include "Editor/Common/DragAndDrop.h"
#include "Core/Math/CommonMath.h"

#include "Editor/Views/Types/Inspector/InspectorView.h"
#include "Editor/Views/Types/SceneView.h"
#include "EclipsedEngine/Scenes/SceneManager.h"

#include "Core/Files/FileWatcher.h"

#include "Core/Settings/EditorSettings.h"

#include "Editor/SelectionContext.h"

#include "Assets/Core/AssetDatabase.h"
#include "Assets/MetaData/Data/TextureMeta.h"
#include "Core/MainSingleton.h"

#include "FontAwesome/7/IconsFontAwesome7.h"

#include "Editor/EditorUI.h"

namespace Eclipse::Editor
{
	void AssetBrowserView::OnOpen()
	{
		auto c = PathManager::GetAssetsPath();

		dirTree = Utilities::DirectoryTree(PathManager::GetAssetsPath());
		Active_View_Node = dirTree.GetRoot();

		engineTree = Utilities::DirectoryTree(PathManager::GetEngineAssetsPath());

		FileWatcher::SubscribeToPath(PathManager::GetEngineAssetsPath(), [this](const FileWatcherEvent& e) {
			shouldReloadAssets = true;
			});

		FileWatcher::SubscribeToPath(PathManager::GetAssetsPath(), [this](const FileWatcherEvent& e) {
			shouldReloadAssets = true;
			});

		ctxMenu.SetActivePath(PathManager::GetAssetsPath());
	}

	void AssetBrowserView::Draw()
	{
		DrawAssetHierachy();
		ImGui::SameLine();
		DrawAssetView();

		ctxMenu.Draw();

		if (shouldReloadAssets)
		{
			//std::filesystem::path LastPath = ctxMenu.GetActivePath();
			const std::filesystem::path lastPath = Active_View_Node->info.filePath;

			shouldReloadAssets = false;
			dirTree.Reload();
			Active_View_Node = dirTree.GetNode(lastPath);

			//ctxMenu.SetActivePath(LastPath);
		}
	}

	bool AssetBrowserView::CheckFileDoubleClicked()
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			return true;
		}

		return false;
	}

	bool AssetBrowserView::CheckFileClicked()
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			return true;
		}

		return false;
	}


#pragma region -- ASSET TREE VIEW

	void AssetBrowserView::DrawAssetHierachy()
	{
		ImGui::BeginChild("FolderStructure", ImVec2(folderStructureWidth, ImGui::GetContentRegionAvail().y), true);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DrawLinesToNodes | ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx(ICON_FA_FOLDER " Assets", base_flags))
		{
			const Utilities::FileNode* projectAssets = dirTree.GetNode(PathManager::GetAssetsPath());
			DrawAssetHierachyEntry(projectAssets);
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx(ICON_FA_FOLDER " Engine Assets", base_flags))
		{
			const Utilities::FileNode* engineAssets = engineTree.GetNode(PathManager::GetEngineAssetsPath());
			DrawAssetHierachyEntry(engineAssets);
			ImGui::TreePop();
		}

		ImGui::PopStyleColor();

		ImGui::EndChild();
	}

	void AssetBrowserView::DrawAssetHierachyEntry(const Utilities::FileNode* node)
	{
		namespace fs = std::filesystem;

		static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DrawLinesToNodes;

		for (Utilities::FileNode* child : node->children)
		{
			std::string name = child->info.filePath.filename().string();
			std::string icon = child->info.GetIcon();

			if (child->isDirectory && ImGui::TreeNodeEx((ICON_FA_FOLDER " " + name).c_str(), node_flags))
			{
				DrawAssetHierachyEntry(child);
				ImGui::TreePop();
			}
			else if (!child->isDirectory && ImGui::Button((icon + " " + name).c_str()))
			{

				Active_Hierarchy_Node = dirTree.GetNode(child->info.filePath);
				ActivePath = child->info.filePath;
				ActiveEntryIndex = entryIndex;

				ctxMenu.SetActivePath(ActivePath);
				SelectionContext::SetActiveContext(AssetTarget(ActivePath));
			}

			if (ImGui::IsItemClicked())
			{
				Active_Hierarchy_Node = dirTree.GetNode(child->info.filePath);
				if (!Active_Hierarchy_Node) Active_Hierarchy_Node = engineTree.GetNode(child->info.filePath);
			}

			if (Active_Hierarchy_Node)
			{
				DragAndDrop::BeginSource(child->info.relativeFilePath.generic_string().c_str(), child->info.relativeFilePath.generic_string().size(), child->info);
			}

		}
	}

#pragma endregion


#pragma region -- ASSET VIEW
	void AssetBrowserView::DrawAssetViewBreadcrumb()
	{
		namespace fs = std::filesystem;

		std::filesystem::path pathCombiner = PathManager::GetAssetsPath();

		fs::path p = std::string("Assets");
		if (Active_View_Node != nullptr) p /= Active_View_Node->info.relativeFilePath.string();

		for (auto& path : p)
		{
			if (path.empty()) return;

			if (path.string() != "Assets")
			{
				ImGui::SameLine();

				ImVec2 pos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(pos.x, pos.y - 2));
				ImGui::PushFont(EditorUI::FontMedium);
				ImGui::Text(">");
				ImGui::PopFont();

				pathCombiner /= path.string();
				//pathCombiner.push_back('/');
				ImGui::SameLine();
			}

			if (ImGui::Button(path.string().c_str()))
			{
				fs::path wantedPath(pathCombiner);
				Active_View_Node = dirTree.GetNode(wantedPath);
			}
		}
	}

	void AssetBrowserView::DrawAssetView()
	{
		namespace fs = std::filesystem;

		ImGui::BeginChild("AssetView");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		DrawAssetViewBreadcrumb();

		ImGui::Separator();

		ImVec2 cursorXY = ImGui::GetCursorPos();


		//ImGui::GetForegroundDrawList()->AddRectFilled(cursorXY, windowSize, ImColor(0, 0, 0, 1));

		ImGui::Indent(10.f);

		for (Utilities::FileNode* node : Active_View_Node->children)
		{
			CheckAssetViewEntryClicked(node);
			entryIndex++;
		}

		ImGui::Unindent(10.f);

		ImGui::PopStyleColor(3);
		ImGui::EndChild();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			ActivePath = "";
			ctxMenu.SetActivePath(ActivePath);
			ActiveEntryIndex = -1;
		}

		entryIndex = 0;
	}

	void AssetBrowserView::DrawAssetViewEntry(const Utilities::FileNode* node)
	{
		float customWidth = ImGui::GetWindowWidth() + scrollBarWidth;

		const float width = customWidth + ImGui::GetWindowPos().x - scrollBarWidth * 2;
		const float buttonSize = 100.0f * myButtonSizeMultiplier;
		const float padding = ImGui::GetStyle().ItemSpacing.x;


		ImVec2 pos = ImGui::GetCursorPos();

		const char* icon = node->info.GetIcon();

		ImGui::PushID((std::string("##") + node->info.filePath.string()).c_str());

		ImVec2 buttonSizeVec(buttonSize, buttonSize);

		ImColor col(1.f, 1.f, 1.f, 1.f);
		if (!ActivePath.empty() && entryIndex == ActiveEntryIndex) col = ImColor(1.00f, 0.75f, 0.30f, 1.00f);

		ImGui::PushFont(EditorUI::FontExtraLarge);
		ImGui::PushStyleColor(ImGuiCol_Text, col.Value);

		if (node->info.type == Utilities::FileInfo::FileType_Texture)
		{
			const IconData& data = TextureIconManager::GetIcon(node->info);

			ImGui::InvisibleButton("##icon", buttonSizeVec);

			ImVec2 p_min = ImGui::GetItemRectMin();
			ImVec2 p_max = ImGui::GetItemRectMax();

			float availW = p_max.x - p_min.x;
			float availH = p_max.y - p_min.y;

			float texWidth = data.width;
			float texHeight = data.height;

			float scale = 0.6f;
			float maxDim = Math::Min(availW, availH) * scale;

			float ratio = texWidth / texHeight;
			float drawWidth, drawHeight;

			if (ratio > 1.0f)
			{
				drawWidth = maxDim;
				drawHeight = maxDim / ratio;
			}
			else
			{
				drawHeight = maxDim;
				drawWidth = maxDim * ratio;
			}

			ImVec2 center((p_min.x + p_max.x) * 0.5f, (p_min.y + p_max.y) * 0.5f);
			ImVec2 imageMin(center.x - drawWidth * 0.5f, center.y - drawHeight * 0.5f);
			ImVec2 imageMax(center.x + drawWidth * 0.5f, center.y + drawHeight * 0.5f);

			Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
			Assets::GUID guid = database.GetGUIDFromFullPath(node->info.filePath);
			Assets::AssetMeta& meta = database.GetProcessedFile(guid);
			Assets::TextureMeta* textureMeta = meta.GetMetaComponent<Assets::TextureMeta>();

			// Write full image
			ImGui::GetWindowDrawList()->AddImage(
				(ImTextureID)data.textureID,
				imageMin,
				imageMax,
				ImVec2(0, 1),
				ImVec2(1, 0),
				col
			);


			// Draw rects
			//for (auto t : textureMeta->spriteRects)
			//{
			//	ImVec2 min = ImVec2(t.rect.position.x / data.fullWidth, t.rect.position.y / data.fullHeight);
			//	ImVec2 max = ImVec2((t.rect.size.x + t.rect.position.x) / data.fullWidth, (t.rect.size.y + t.rect.position.y) / data.fullHeight);

			//	ImGui::GetWindowDrawList()->AddImage(
			//		(ImTextureID)data.textureID,
			//		imageMin,
			//		imageMax,
			//		min,
			//		max,
			//		col
			//	);
			//}
		}
		else
		{
			ImGui::Button(icon, buttonSizeVec);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_Childing_Reordering"))
			{
				IM_ASSERT(payload->DataSize == sizeof(unsigned));
				unsigned draggedEntityID = *reinterpret_cast<unsigned*>(payload->Data);

				std::filesystem::path pathToPlacePrefab;
				if (node->isDirectory)
					pathToPlacePrefab = node->info.filePath;
				else
					pathToPlacePrefab = PathManager::GetAssetsPath();

				//HierarchyView::CreatePrefab(draggedEntityID, pathToPlacePrefab);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleColor();
		ImGui::PopFont();

		if (node != nullptr)
		{
			DragAndDrop::BeginSource(node->info.relativeFilePath.string().c_str(), node->info.relativeFilePath.string().size(), node->info);
		}

		std::string label = node->info.filePath.filename().string();
		const float maxSize = buttonSize - 86.f;
		if (label.size() > maxSize)
		{
			label.erase(maxSize, label.size());
			label += "...";
		}

		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		if (entryIndex == ActiveEntryIndex)
		{
			ImGui::GetWindowDrawList()->AddRect(min, max, ImColor(1.00f, 0.75f, 0.30f, 1.00f), 6.f);
			ImGui::GetWindowDrawList()->AddRectFilled(min, max, ImColor(1.00f, 0.75f, 0.30f, 0.2f), 6.f);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::GetWindowDrawList()->AddRect(min, max, ImColor(0.95f, 0.60f, 0.20f, 0.85f), 6.f);
			ImGui::GetWindowDrawList()->AddRectFilled(min, max, ImColor(0.95f, 0.60f, 0.20f, 0.2f), 6.f);
		}

		ImVec2 center = ImVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);

		ImGui::PushFont(EditorUI::FontTiny);
		ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
		const char* end = label.c_str() + 10;
		ImGui::GetWindowDrawList()->AddText(
			EditorUI::FontTiny,
			0.0f,
			ImVec2(center.x - textSize.x * 0.5f, max.y - textSize.y - 6),
			col,
			label.c_str()
		);

		ImGui::PopFont();


		ImGui::PopID();

		if (ImGui::IsItemHovered() && (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
		{
			ActivePath = node->info.filePath;
			SelectionContext::SetActiveContext(AssetTarget(ActivePath));

			ActiveEntryIndex = entryIndex;

			ctxMenu.SetActivePath(ActivePath);
		}

		float currentWidth = ImGui::GetItemRectMax().x;

		if (currentWidth + buttonSize + padding < width)
		{
			ImGui::SameLine();
		}
	}

	void AssetBrowserView::CheckAssetViewEntryClicked(const Utilities::FileNode* node)
	{
		DrawAssetViewEntry(node);

		if (!CheckFileDoubleClicked()) return;

		if (node->isDirectory)
		{
			ActivePath = node->info.filePath;
			Active_View_Node = dirTree.GetNode(node->info.filePath);
		}
		else
		{
			ActivePath = node->info.filePath;
			OpenFile(node->info);
		}
	}
#pragma endregion

	void AssetBrowserView::OpenFile(const Utilities::FileInfo& fifo)
	{
		switch (fifo.type)
		{
		case Utilities::FileInfo::FileType_Scene:
		{
			Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
			Assets::GUID guid = database.GetGUIDFromFullPath(fifo.filePath);

			SceneManager::LoadScene(guid);
			Settings::EditorSettings::SetLastActiveScene(fifo.filePath.stem().filename().string());

			SceneView::ResetCamera();
		}
		break;
		case Utilities::FileInfo::FileType_Prefab:
		{
			std::ifstream stream(fifo.filePath);
			if (!stream.is_open())
				return;

			SceneManager::UnloadScene();
			InitNewPhysicsScene();

			SceneManager::SetActiveSceneType(SceneManager::Prefab);
			SceneManager::SetActiveScene(fifo.filePath.generic_string().c_str());


			size_t prefSize = std::filesystem::file_size(fifo.filePath);
			char* data = reinterpret_cast<char*>(malloc(prefSize + 1));
			stream.read(data, prefSize);
			memset(data + prefSize, '\0', 1);
			stream.close();

			SceneManager::ActivePrefabEditSceneID = CreateGameobjectFunc(data);

			SceneView::ResetCamera();

			free(data);
		}
		break;

		default:
			system(fifo.filePath.string().c_str());
			break;
		}
	}
}