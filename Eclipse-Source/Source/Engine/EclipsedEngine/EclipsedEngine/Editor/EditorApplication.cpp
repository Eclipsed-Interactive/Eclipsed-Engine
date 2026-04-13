#include "EditorApplication.h"

#ifdef ECLIPSED_EDITOR

#include "EclipsedEngine/Editor/EditorProjectManager.h"
#include "CoreEngine/PathManager.h"

namespace Eclipse::Editor
{
	void EditorApplication::Init()
	{
		if (!PathManager::ProjectSet())
		{
			std::string projectPath = EditorProjectManager::LoadOrSelectProject();
			myContext.Init(projectPath);
		}
		else
		{
			myContext.Init(PathManager::GetProjectPath().string());
		}
	}

	int EditorApplication::Update()
	{
		int shouldClose = myContext.BeginFrame();
		myContext.Update();
		myContext.Render();
		myContext.EndFrame();

		return shouldClose;
	}

	void EditorApplication::Shutdown()
	{
		myContext.Shutdown();
	}
}
#endif