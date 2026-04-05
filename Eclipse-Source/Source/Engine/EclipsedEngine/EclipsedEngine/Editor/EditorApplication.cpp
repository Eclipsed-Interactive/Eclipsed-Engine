#include "EditorApplication.h"

#ifdef ECLIPSED_EDITOR

#include "EclipsedEngine/Editor/EditorProjectManager.h"


namespace Eclipse::Editor
{
    void EditorApplication::Init()
    {
        std::string projectPath = EditorProjectManager::LoadOrSelectProject();

        myContext.Init(projectPath);
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