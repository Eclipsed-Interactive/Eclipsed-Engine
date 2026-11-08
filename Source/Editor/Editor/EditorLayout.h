#pragma once

#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui_internal.h"
#include <vector>

namespace Eclipse::Editor
{
	class EditorLayout
	{
	public:
		static void Init();
        static void Shutdown();

		static void EndFrame();

		static void Draw();
		static void OpenLayout(std::string_view layoutName);

		static void RegisterViewTypes();
		static void OpenView(std::string_view viewName, int id = -1);

		static EditorViewRegistry& GetViewRegistry();

    public:
        static void SaveLayout(const char* layoutPath);
		static void SaveNewLayout(std::string_view layoutName);


    public:
        static bool LoadLayout(const char* layout);
        static bool LoadLayoutFromMemory(const char* layout);
        static bool LoadLayoutFromDisk(const char* path);
        static bool LoadLayoutFromProject(const char* layoutName);

    private:
		static void Internal_LoadLayoutFromMemory(const char* layout);
		static void Internal_LoadLayoutFromDisk(const char* path);
		static void Internal_LoadLayoutFromProject(const char* layoutName);

	private:
		static inline EditorViewRegistry registry;
		static inline std::vector<EditorViewInstance*> activeViews;






	public:
	private:
        static inline std::string layoutToOpen = "";
        static inline const char* DefaultLayout = R"json({
    "ini": "[Window][WindowOverViewport_11111111]\nPos=0,19\nSize=1920,990\nCollapsed=0\n\n[Window][Game##1559288921]\nPos=265,52\nSize=674,434\nCollapsed=0\nDockId=0x00000003,1\n\n[Window][Scene##-692680716]\nPos=265,52\nSize=674,434\nCollapsed=0\nDockId=0x00000003,0\n\n[Window][Asset Browser##-1907583850]\nPos=0,488\nSize=939,232\nCollapsed=0\nDockId=0x00000005,0\n\n[Window][Hierarchy##263358423]\nPos=0,52\nSize=263,434\nCollapsed=0\nDockId=0x00000002,0\n\n[Window][Inspector##1340586163]\nPos=941,52\nSize=339,668\nCollapsed=0\nDockId=0x00000007,0\n\n[Window][Debug##Default]\nPos=60,60\nSize=400,400\nCollapsed=0\n\n[Window][##MainMenuBar]\nSize=1920,19\nCollapsed=0\n\n[Window][Console##2063189080]\nPos=0,488\nSize=939,232\nCollapsed=0\nDockId=0x00000005,1\n\n[Docking][Data]\nDockSpace       ID=0x538DB22F Window=0xEF43222D Pos=60,135 Size=1280,668 Split=X\n  DockNode      ID=0x00000006 Parent=0x538DB22F SizeRef=939,668 Split=Y\n    DockNode    ID=0x00000004 Parent=0x00000006 SizeRef=1280,434 Split=X\n      DockNode  ID=0x00000002 Parent=0x00000004 SizeRef=263,668 Selected=0xB8268379\n      DockNode  ID=0x00000003 Parent=0x00000004 SizeRef=1015,668 CentralNode=1 Selected=0x7E2AA325\n    DockNode    ID=0x00000005 Parent=0x00000006 SizeRef=1280,232 Selected=0x4927D1AC\n  DockNode      ID=0x00000007 Parent=0x538DB22F SizeRef=339,668 Selected=0xC5102FD1\n\n",
    "views": [
        {
            "name": "Game",
            "id": 1559288921
        },
        {
            "name": "Scene",
            "id": -692680716
        },
        {
            "name": "Asset Browser",
            "id": -1907583850
        },
        {
            "name": "Hierarchy",
            "id": 263358423
        },
        {
            "name": "Inspector",
            "id": 1340586163
        },
        {
            "name": "Console",
            "id": 2063189080
        }
    ]
})json";

	};
}