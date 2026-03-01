#include "EclipsedEngine/Editor/EditorApplication.h"
#include "EclipsedEngine/EclipsedRuntime.h"
#include <windows.h>

#include <filesystem>

int main()
{
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_SHOW); 


#ifdef ECLIPSED_EDITOR
    Eclipse::Editor::EditorApplication editorApplication;

    editorApplication.Init();

    while (editorApplication.Update());

    editorApplication.Shutdown();
#else 
    Eclipse::EclipsedRuntime eclipseRuntime = {};

    eclipseRuntime.StartEngine();

    while (eclipseRuntime.BeginFrame()) {
        eclipseRuntime.Update();
        eclipseRuntime.UpdateGame();
        eclipseRuntime.Render();
        eclipseRuntime.EndFrame();
    }

    eclipseRuntime.Shutdown();
#endif
}