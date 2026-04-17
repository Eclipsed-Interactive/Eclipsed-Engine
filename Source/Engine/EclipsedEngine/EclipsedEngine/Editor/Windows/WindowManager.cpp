#include "WindowManager.h"

#include "ImGui/imgui.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include <fstream>

#include "EclipsedEngine/Editor/Windows/WindowTypes/Window.h"
#include "EclipsedEngine/Editor/ImGui/ImGui_Impl.h"

#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/Settings/EngineSettings.h"

#include "EclipsedEngine/Replication/ReplicationManager.h"

#include "EclipsedEngine/Editor/Windows/WindowData.h"
//#include "CoreEngine/Settings/SettingsRegistry.h"

#include "CoreEngine/Settings/EditorSettings.h"

#include "CoreEngine/Debug/DebugLogger.h"

#include "CoreEngine/Files/FileUtilities.h"

#include "EclipsedEngine/Editor/Layout/LayoutManager.h"
#include "EclipsedEngine/Editor/Game/GameCompiler.h"
#include "EclipsedEngine/Editor/Game/GameLoader.h"

namespace Eclipse::Editor
{
	void WindowManager::OpenWindow(const std::string& name, int aId)
	{
		AbstractWindow* window = static_cast<AbstractWindow*>(WindowRegistry::GetWindow(name));
		if (window == nullptr) return;


		AbstractWindow* newWindow = window->GetNewWindow(aId);

		auto openWindows = Settings::EditorSettings::GetCurrentlyOpenEditorWindows();
		openWindows.push_back({ newWindow->instanceID, newWindow->windowName });
		Settings::EditorSettings::SetCurrentlyOpenEditorWindows(openWindows);

		newWindow->Open();
		IdToWindow[newWindow->instanceID] = newWindow;

		WindowData data;
		data.id = newWindow->instanceID;
		data.name = newWindow->windowName;

		//Settings::SettingsRegistry::AddToList("editor.open-windows", data);
	}
	void WindowManager::UpdateMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			{ // Left menu
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::BeginMenu("Game")) {
						if (ImGui::MenuItem("Generate Game"))
						{
							auto te = ("cd " + (PathManager::GetProjectRoot() / "Tools").generic_string() + " && "
								"generate-game-editor.bat " + PathManager::GetProjectRoot().generic_string() + " " + PathManager::GetEngineRoot().parent_path().generic_string());

							system(("cd " + (PathManager::GetProjectRoot() / "Tools").generic_string() + " && "
								"generate-game-editor.bat " + PathManager::GetProjectRoot().generic_string() + " " + PathManager::GetEngineRoot().parent_path().generic_string()).c_str());
						}
						if (ImGui::MenuItem("Open Game SLN"))
						{
							system(("cd " + (PathManager::GetProjectRoot() / "Library/Engine-Build").generic_string() + " && start Eclipsed-Game.sln").c_str());
						}

						if (ImGui::MenuItem("Recompile Game DLL"))
						{
							GameLoader::UnloadGameDLL();
							GameCompiler::CompileGame();
							GameLoader::LoadGameDLL();
						}

						if (ImGui::MenuItem("Build Game Release EXE"))
						{
							std::filesystem::path CDPath = PathManager::GetEngineRoot().parent_path().parent_path() / "Tools";
							system(("cd " + CDPath.generic_string() + " && start build-game-editor.bat Release").c_str());
						}
						// else if (ImGui::MenuItem("Build Game Debug EXE"))
						// {
						// 	std::filesystem::path CDPath = PathManager::GetEngineRoot().parent_path().parent_path() / "Tools";
						// 	system(("cd " + CDPath.generic_string() + " && start build-game-editor.bat Debug").c_str());
						// }

						ImGui::EndMenu();
					}



					ImGui::EndMenu();
				}


				if (ImGui::BeginMenu("Windows"))
				{
					for (const auto& [name, window] : WindowRegistry::GetWindows())
					{
						if (!window->GetCategoryName())
						{
							if (ImGui::MenuItem(name.c_str())) OpenWindow(name.c_str(), -1);
						}
						else
						{
							std::string fullCategory = window->GetCategoryName();

							std::vector<std::string> categories = {};
							while (!fullCategory.empty())
							{
								size_t idx = fullCategory.find_first_of('/');
								std::string category = fullCategory.substr(0, idx);

								if (!category.empty())
									categories.push_back(category);

								if (idx == std::string::npos)
									break;

								fullCategory.erase(0, idx + 1);
							}

							AddWindowToCategory(categories, 0, name);
						}
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Network"))
				{
					ImGui::Text("IP: ");
					ImGui::SameLine();

					char IP[17];
					memcpy(IP, Replication::ReplicationManager::IP.c_str(), Replication::ReplicationManager::IP.size() + 1);
					if (ImGui::InputText("##IpToUse", IP, 16))
					{
						std::ofstream stream("NetworkIp.ntwrk");
						stream << IP;
						stream.close();

						Replication::ReplicationManager::IP = IP;
					}

					ImGui::Checkbox("##Start Server Checkbox", &Replication::ReplicationManager::startServer);
					ImGui::SameLine();
					ImGui::Text("Start Server");

					ImGui::Checkbox("##Start Client Checkbox", &Replication::ReplicationManager::startClient);
					ImGui::SameLine();
					ImGui::Text("Start Client");

					ImGui::EndMenu();
				}
			}




			int size = ImGui::CalcTextSize("Layout: ").x + 50;
			ImGui::SameLine(ImGui::GetWindowWidth() - (size * 2) - 50);
			ImGui::SetNextItemWidth(size * 2);

			if (ImGui::BeginCombo("##Layouts", std::string("Layout: " + LayoutManager::GetActiveLayoutName()).c_str()))
			{
				if (ImGui::Button("Import"))
				{
					LayoutManager::ImportLayout();
				}

				ImGui::SameLine();
				if (ImGui::Button("Save"))
				{
					LayoutManager::SaveActiveLayout();
				}

				ImGui::SameLine();
				if (ImGui::Button("New"))
				{
					LayoutManager::SaveAsNewLayout([this]() {
						this->OpenLayout(LayoutManager::GetNewLayoutName().c_str());
						});
				}

				ImGui::SeparatorText("Layouts");

				for (auto& layout : LayoutManager::GetLayouts())
				{
					bool isSelected = LayoutManager::GetActiveLayoutName() == layout;
					if (isSelected) continue;

					if (ImGui::Selectable(layout.c_str(), isSelected))
					{
						OpenLayout(layout.c_str());
					}
				}

				ImGui::EndCombo();
			}

			//if (ImGui::Button("Debug"))
			//{
			//	myShowDebugWindow = !myShowDebugWindow;
			//}

			ImGui::EndMainMenuBar();
		}
	}

	void WindowManager::Update()
	{
		UpdateMainMenuBar();

		LayoutManager::Update();

		for (auto it = IdToWindow.begin(); it != IdToWindow.end(); )
		{
			const int& id = it->first;
			AbstractWindow* window = it->second;

			if (window->myWasLastOpen != window->myIsOpen)
			{
				delete window;
				it = IdToWindow.erase(it);
			}
			else
			{
				std::string imguiWindowName = window->windowName + "##" + std::to_string(id);
				ImGui::PushID(id);
				ImGui::Begin(imguiWindowName.c_str(), &window->myIsOpen, window->flags);
				window->PreUpdate();
				window->Update();
				window->PostUpdate();
				ImGui::End();
				ImGui::PopID();

				if (!window->myIsOpen)
				{
					auto openWindows = Settings::EditorSettings::GetCurrentlyOpenEditorWindows();
					for (auto it = openWindows.begin(); it != openWindows.end();)
					{
						if (it->ID == window->instanceID) {
							openWindows.erase(it);
							Settings::EditorSettings::SetCurrentlyOpenEditorWindows(openWindows);
							Settings::EditorSettings::Save();
							break;
						}

						it++;
					}
					Settings::EditorSettings::SetCurrentlyOpenEditorWindows(openWindows);
				}

				++it;
			}
		}
	}

	void WindowManager::Begin()
	{
		Settings::EditorSettings::SetCurrentlyOpenEditorWindows({});

		LayoutManager::LoadLayouts();
		OpenLayout("CurrentLayout");

		ImGui::LoadIniSettingsFromDisk("imgui.ini");
		using namespace rapidjson;

		//const std::vector<Settings::OpenEditorWindows>& openWindows = Settings::EditorSettings::GetCurrentlyOpenEditorWindows();

		//for (const Settings::OpenEditorWindows& openWindow : Settings::EditorSettings::GetCurrentlyOpenEditorWindows())
		//{
		//	OpenWindow(openWindow.Name.c_str(), openWindow.ID);
		//}
	}

	void WindowManager::End()
	{
		LayoutManager::SaveLayout();
	}

	void WindowManager::AddWindowToCategory(const std::vector<std::string>& categories, size_t idx, const std::string& windowName)
	{
		if (idx >= categories.size()) return;

		if (ImGui::BeginMenu(categories[idx].c_str()))
		{
			if (idx == categories.size() - 1)
			{
				if (ImGui::MenuItem(windowName.c_str()))
				{
					OpenWindow(windowName, -1);
				}
			}
			else
			{
				AddWindowToCategory(categories, idx + 1, windowName);
			}

			ImGui::EndMenu();
		}
	}
	void WindowManager::OpenNewLayout()
	{
	}
	void WindowManager::OpenLayout(const char* layout)
	{
		const auto& layoutWindows = LayoutManager::OpenLayout(layout);

		for (auto& window : IdToWindow)
		{
			window.second->myIsOpen = false;
			Settings::EditorSettings::SetCurrentlyOpenEditorWindows({});
		}

		for (const auto& window : layoutWindows)
		{
			OpenWindow(window.name.c_str(), window.id);
		}
	}
}