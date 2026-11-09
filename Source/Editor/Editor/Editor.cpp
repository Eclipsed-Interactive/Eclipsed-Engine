#include "Editor.h"

#include "Views/ViewManager.h"
#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui.h"

#include "MainMenuBar.h"
#include "ProjectManager.h"

#include "Assets/AssetImporter.h"
#include "Core/PathManager.h"

#include "EditorLayout.h"
#include "ToolBar.h"
#include "StatusBar.h"
#include "DockSpace.h"
#include "EditorUI.h"

#include "Editor/Game/GameModuleManager.h"
#include "Reflection/Reflection.hpp"

#include "Core/Standard/Serialization/Serialization.hpp"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "Editor/Common/EditorActions.h"
#include "Core/Settings/EditorSettings.h"

#include "PlayModeController.h"

#include <Core/Logger/DebugLogger.h>

namespace Eclipse::Editor
{
	void Editor::EarlyInit()
	{
		LOG("The editor is initialized.");
		LOG_WARNING("The editor is also uninitialized!");
		LOG_ERROR("The Editor FAILED to open the required president of component.");

		ProjectManager::LoadOrSelectProject();
	}

	void Editor::Init()
	{
		const auto now = std::chrono::system_clock::now();
		const auto local = std::chrono::zoned_time{ std::chrono::current_zone(), now }.get_local_time();
		const auto sinceMidnight = std::chrono::floor<std::chrono::seconds>(local - std::chrono::floor<std::chrono::days>(local));
		const auto t = sinceMidnight.count();

		Engine::Init();

		ImGui::SetCurrentContext((ImGuiContext*)Engine::GetImGuiContext());

		Assets::AssetImporter::ImportAssets(PathManager::GetEngineAssetsPath(), "Engine Assets");

		EditorUI::Init();
		EditorLayout::Init();

		GameModuleManager::TryCompileAndLoad();

		if (Settings::EditorSettings::GetOpenLastOpenSceneOnStartup())
			SceneManager::LoadLastActive();

		Engine::LateInit();
	}

	bool Editor::ShouldClose()
	{
		return Engine::ShouldClose();
	}

	void Editor::Shutdown()
	{
		Engine::Shutdown();
		EditorLayout::Shutdown();
	}

	void Editor::BeginFrame()
	{
		PlayModeController::ApplyMode();

		Engine::BeginFrame();
		EditorActions::Update();
	}

	void Editor::Update()
	{
		LOG_ERROR("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum eros magna, vehicula sed quam sed, tincidunt tempus enim. Suspendisse et aliquam odio, a fringilla ante. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nullam non tempus magna, eu consectetur purus. Nullam commodo ante elementum lorem viverra hendrerit. Proin lectus odio, porta at dignissim at, accumsan ac orci. Nulla commodo eros eget ornare pharetra. Morbi eu felis non nisl elementum condimentum. Curabitur neque enim, imperdiet ac dictum non, vestibulum egestas tellus. Etiam at nisi id arcu efficitur maximus. Donec nec lacus fringilla, laoreet nulla sed, fermentum sapien. Suspendisse dignissim ipsum in mi ornare, eu lobortis diam venenatis. Mauris malesuada feugiat turpis, vitae consectetur erat efficitur eu. Integer ut erat quis nisi tempor auctor non ut mi. Curabitur quam nulla, imperdiet ac iaculis nec, tempor eu est. Etiam tellus elit, semper ut malesuada nec, cursus vel massa.");

		Engine::Update();

		if (PlayModeController::ShouldTick())
		{
			Engine::UpdateGame();
		}
	}

	void Editor::Render()
	{
		Engine::Render();

		MainMenuBar::Draw();
		ToolBar::Draw();
		StatusBar::Draw();
		DockSpace::Draw();
		EditorLayout::Draw();
	}

	void Editor::EndFrame()
	{
		Engine::LateRender();
		Engine::EndFrame();

		EditorLayout::EndFrame();

		if (!PlayModeController::IsInPlaySession() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			SceneManager::CheckDirty();
		}
	}
}