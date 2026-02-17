#include "AssetWindowContextMenu.h"

#include <filesystem>
#include "ImGui/imgui.h"

#include "CoreEngine/Debug/DebugLogger.h"
#include "CoreEngine/PathManager.h"

#include <fstream>

//#include <shellapi.h>

#include "EclipsedEngine/Editor/Game/GameLoader.h"
#include "EclipsedEngine/Editor/Game/GameCompiler.h"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "EntityEngine/ComponentManager.h"

#include "EclipsedEngine/Input/Input.h"

namespace Eclipse::Editor
{
	AssetWindowContextMenu::AssetWindowContextMenu() : AbstractContextMenu("AssetsCtxMenu") {}



	const std::filesystem::path& AssetWindowContextMenu::GetActivePath()
	{
		return activePath;
	}
	void AssetWindowContextMenu::SetActivePath(const std::filesystem::path& aPath)
	{
		activePath = aPath;
	}

	void AssetWindowContextMenu::UpdateAlways()
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

			if (ImGui::Button("OK") || Input::GetKeyDown(Keycode::ENTER))
			{
				std::string renamePathExt = activePathAtRenaming.extension().string();

				memcpy(tempName + strlen(tempName), renamePathExt.c_str(), renamePathExt.size() + 1);

				std::filesystem::path newName = activePathAtRenaming.parent_path() / tempName;

				std::filesystem::rename(activePathAtRenaming, newName);

				ImGui::CloseCurrentPopup();
				renameModal = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel") || Input::GetKeyDown(Keycode::ESCAPE))
			{
				ImGui::CloseCurrentPopup();
				renameModal = false;
			}

			ImGui::EndPopup();
		}
	}

	void AssetWindowContextMenu::Update()
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
			std::string sceneName = SceneManager::GetActiveScene();
			ComponentManager::Clear();

			GameLoader::UnloadGameDLL();

			std::filesystem::remove(PathManager::GetGameDllBuildPath() / "Game.dll");
			std::filesystem::remove(PathManager::GetGameDllBuildPath() / "Game.pdb");

			GameCompiler::CompileGame();
			GameLoader::LoadGameDLL();

			SceneManager::LoadScene(sceneName);
		}
	}

	void AssetWindowContextMenu::CreateMenu()
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
			std::ofstream otu(path);

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