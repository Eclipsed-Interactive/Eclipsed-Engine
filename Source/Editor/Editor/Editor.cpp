#include "Editor.h"

#include "Views/ViewManager.h"
#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui.h"

#include "MainMenuBar.h"
#include "ProjectManager.h"

#include "Assets/AssetImporter.h"
#include "Core/PathManager.h"

#include "EditorLayout.h"
#include "EditorUI.h"

#include "Editor/Game/GameModuleManager.h"
#include "Reflection/Reflection.hpp"

#include "Core/Standard/Serialization/Serialization.hpp"

namespace Eclipse::Editor
{
	void Editor::EarlyInit()
	{
		ProjectManager::LoadOrSelectProject();

		float value = Serialization::FromJson<float>("3.1415");
	}

	void Editor::Init(void* imguiCtx)
	{
		ImGui::SetCurrentContext((ImGuiContext*)imguiCtx);

		Assets::AssetImporter::ImportAssets(PathManager::GetEngineAssetsPath(), "Engine Assets");

		EditorUI::Init();
		EditorLayout::Init();

		GameModuleManager::TryCompileAndLoad();
	}

	void Editor::Shutdown()
	{
		EditorLayout::Shutdown();
	}

	void Editor::BeginFrame()
	{

	}
	
	void Editor::Render()
	{
		MainMenuBar::Draw();
		EditorLayout::Draw();
		//ViewManager::Update();
	}
	
	void Editor::EndFrame()
	{
		EditorLayout::EndFrame();
	}
}