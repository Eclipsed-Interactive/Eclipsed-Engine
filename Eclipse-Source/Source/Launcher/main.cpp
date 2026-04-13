#include "EclipsedEngine/Editor/EditorApplication.h"
#include "CoreEngine/PathManager.h"
#include "EclipsedEngine/EclipsedRuntime.h"
#include <windows.h>

#include "CoreEngine/Debug/DebugLogger.h"

#include <filesystem>
#include <string>


int main(int argc, char* argv[])
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    std::string projectPath = "";
    if (argc > 1) // Engine opened with project path.
    {
        std::ofstream file("ecl_testing.txt");
        file.write(argv[1], strlen(argv[1]));

        projectPath = argv[1];

        Eclipse::PathManager::Init(projectPath);
    }
    else // Engine tries to use the stored path in the .ini file.
    {
        if (std::filesystem::exists(".ini"))
        {
            std::ifstream file(".ini");

            file.seekg(0, std::ios::end);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            projectPath.resize(size);
            file.read(projectPath.data(), size);

            Eclipse::PathManager::Init(projectPath);
        }
    }



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