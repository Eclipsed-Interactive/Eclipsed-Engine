#ifdef ECL_EDITOR
#include "AssetBrowserViewContextMenu.h"

#include <filesystem>
#include "ImGui/imgui.h"

#include "EclipsedEngine/DebugLogger.h"

#include "Core/PathManager.h"

#include <fstream>

//#include <shellapi.h>

#include "Editor/Game/GameLoader.h"
#include "Editor/Game/GameCompiler.h"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "ECS/ComponentManager.h"

#include "Input/Input.h"

#include "Editor/Game/GameCompiler.h"

#include "Assets/BinaryFormats/MaterialData.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

namespace Eclipse::Editor
{
	AssetBrowserViewContextMenu::AssetBrowserViewContextMenu() : AbstractContextMenu("AssetsCtxMenu") {}



	const std::filesystem::path& AssetBrowserViewContextMenu::GetActivePath()
	{
		return activePath;
	}
	void AssetBrowserViewContextMenu::SetActivePath(const std::filesystem::path& aPath)
	{
		activePath = aPath;
	}

	void AssetBrowserViewContextMenu::UpdateAlways()
	{
		if (renameModal) {
			ImGui::OpenPopup("SetName");
		}

		if (ImGui::BeginPopupModal("SetName", &renameModal, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (focusNameRename) {
				focusNameRename = false;
				ImGui::SetKeyboardFocusHere();
			}

			ImGui::InputText("New name", tempName, 512);

			if (ImGui::Button("OK") || Input::Input::GetKeyDown(Keycode::ENTER))
			{
				std::string renamePathExt = activePathAtRenaming.extension().string();

				memcpy(tempName + strlen(tempName), renamePathExt.c_str(), renamePathExt.size() + 1);

				std::filesystem::path newName = activePathAtRenaming.parent_path() / tempName;

				std::filesystem::rename(activePathAtRenaming, newName);

				ImGui::CloseCurrentPopup();
				renameModal = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel") || Input::Input::GetKeyDown(Keycode::ESCAPE))
			{
				ImGui::CloseCurrentPopup();
				renameModal = false;
			}

			ImGui::EndPopup();
		}
	}

	void AssetBrowserViewContextMenu::Update()
	{
		if (ImGui::BeginMenu("Create"))
		{
			CreateMenu();
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Show in Explorer"))
		{
			std::string command = "/select,\"" + activePath.string() + "\"";
			//ShellExecute(NULL, "open", "explorer.exe", command.c_str(), NULL, 10);
		}

		if (activePath.empty()) ImGui::BeginDisabled();

		if (ImGui::MenuItem("Rename"))
		{
			activePathAtRenaming = activePath;
			std::string str = activePathAtRenaming.filename().replace_extension().string();
			memcpy(tempName, str.c_str(), str.size() + 1);

			renameModal = true;
			focusNameRename = true;
		}

		if (ImGui::MenuItem("Open"))
		{
			//ShellExecute(NULL, "open", activePath.string().c_str(), NULL, NULL, 10);
		}

		if (ImGui::MenuItem("Delete"))
		{
			std::filesystem::remove(activePath);
		}

		if (activePath.empty()) ImGui::EndDisabled();

		if (ImGui::MenuItem("Copy Path"))
		{
			LOG_ERROR("Copy Path not implemented.");
		}

		if (ImGui::MenuItem("Recompile"))
		{
			//GameModuleManager::CompileAndLoad();
		}
	}

	void AssetBrowserViewContextMenu::CreateMenu()
	{
		if (ImGui::MenuItem("Folder"))
		{
			std::filesystem::path savedPath;
			if (activePath.has_extension())
			{
				activePath.remove_filename();
				savedPath = activePath;
			}
			else
				savedPath = activePath;

			std::filesystem::create_directory(PathManager::GetAssetsPath() / activePath / "NewFolder");

			activePath = savedPath;
		}

		if (ImGui::MenuItem("Material"))
		{
			std::filesystem::path savedPath;
			if (activePath.has_extension())
			{
				activePath.remove_filename();
				savedPath = activePath;
			}
			else
				savedPath = activePath;

			std::string path = (activePath / "Test.mat").generic_string();

			Assets::SerializableMaterialData matData;
			std::ofstream of(path, std::ios::binary);

			cereal::JSONOutputArchive ar(of);
			ar(matData);

			activePath = savedPath;
		}

		if (ImGui::MenuItem("Component"))
		{
			std::filesystem::path savedPath;
			if (activePath.has_extension())
			{
				activePath.remove_filename();
				savedPath = activePath;
			}
			else
				savedPath = activePath;

			LOG_ERROR("Creating Component not implemented.");

			activePath = savedPath;
		}

		if (ImGui::MenuItem("Scene"))
		{
			std::filesystem::path savedPath;
			if (activePath.has_extension())
			{
				activePath.remove_filename();
				savedPath = activePath;
			}
			else
				savedPath = activePath;

			std::ofstream outputStream(PathManager::GetAssetsPath() / activePath / "NewScene.scene");
			outputStream.close();

			activePath = savedPath;
		}

		if (ImGui::MenuItem("Physics Material"))
		{
			std::filesystem::path savedPath;
			if (activePath.has_extension())
			{
				activePath.remove_filename();
				savedPath = activePath;
			}
			else
				savedPath = activePath;

			LOG_ERROR("Creating Physics Material not implemented.");

			activePath = savedPath;
		}

		// if (ImGui::MenuItem("Prefab"))
		// {
		// 	std::filesystem::path savedPath;
		// 	if (activePath.has_extension())
		// 	{
		// 		activePath.remove_filename();
		// 		savedPath = activePath;
		// 	}
		// 	else
		// 		savedPath = activePath;

		// 	std::ofstream outputStream(activePath / "New prefab.eprf");

		// 	outputStream << "Test";
		// 	outputStream.close();

		// 	activePath = savedPath;
		// }
	}
}
#endif