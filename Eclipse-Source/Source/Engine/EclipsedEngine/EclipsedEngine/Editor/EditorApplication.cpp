#include "EditorApplication.h"

#ifdef ECLIPSED_EDITOR

#include "EclipsedEngine/Editor/EditorProjectManager.h"
#include "CoreEngine/PathManager.h"

namespace Eclipse::Editor
{
	void EditorApplication::Init(const char* aPath)
	{
		if (!aPath)
		{
			std::string projectPath = EditorProjectManager::LoadOrSelectProject();
			myContext.Init(projectPath);
		}
		else
		{
			myContext.Init(aPath);
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