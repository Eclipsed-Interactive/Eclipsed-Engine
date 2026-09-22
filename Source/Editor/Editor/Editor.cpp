#include "Editor.h"

#include "Views/ViewManager.h"
#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui.h"

//#include "Views/Types/AssetBrowserView.h"

#include "MainMenuBar.h"
#include "ProjectManager.h"

#include "Assets/AssetImporter.h"
#include "Core/PathManager.h"

#include "EditorLayout.h"
#include "EditorUI.h"

#include "Editor/Game/GameModuleManager.h"
#include "Reflection/Reflection.h"

namespace Eclipse::Editor
{
	void Editor::EarlyInit()
	{
		ProjectManager::LoadOrSelectProject();
	}

	void Editor::Init(void* imguiCtx)
	{
		ImGui::SetCurrentContext((ImGuiContext*)imguiCtx);

		Assets::AssetImporter::ImportAssets(PathManager::GetEngineAssetsPath(), "Engine Assets");

		EditorUI::Init();
		EditorLayout::Init();

		//Reflection::Reflect(PathManager::GetProjectRoot() / "Generated" / "Reflection");
		//GameModuleManager::Load("C:/Users/zulto/Desktop/GamePlugin");
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