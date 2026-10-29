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

namespace Eclipse::Editor
{
	void Editor::EarlyInit()
	{
		ProjectManager::LoadOrSelectProject();
	}

	void Editor::Init()
	{
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
		Engine::Update();

		if (PlayModeController::ShouldTick())
			Engine::UpdateGame();
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
	}
}