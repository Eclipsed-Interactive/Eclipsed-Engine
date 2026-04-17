#include "EditorContext.h"

#include "AssetEngine/Editor/Importer/EditorAssetImporter.h"
#include "EclipsedEngine/Editor/Common/EditorActions.h"


#ifdef ECLIPSED_EDITOR
namespace Eclipse::Editor
{
	void EditorContexts::Init(const std::string& projectPath)
	{
		myRuntime.Init(projectPath);
		myUIManager.Init();
	}

	bool EditorContexts::BeginFrame()
	{
		myUIManager.BeginFrame();
		return myRuntime.BeginFrameEngine();
	}

	void EditorContexts::Update()
	{
		EditorActions::Update();
		
		myRuntime.UpdateEngine();

		if (myRuntime.IsPlaying())
		{
			myRuntime.UpdateGame();
		}

		myRuntime.RenderEngine();
		myUIManager.Update();
	}

	void EditorContexts::Render()
	{
		myUIManager.Render();
	}

	void EditorContexts::EndFrame()
	{	
		myRuntime.EndFrameEngine();
	}

	void EditorContexts::Shutdown()
	{
		myRuntime.Shutdown();
		myUIManager.Shutdown();
	}
}
#endif