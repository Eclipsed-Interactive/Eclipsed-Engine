#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/External/implot.h"

#include "Core/PathManager.h"
#include <string>

namespace Eclipse::Editor
{
    namespace ImGui_Impl
    {
        inline std::string currentEditorLayout = "Default";

        inline void ImplementImGui(GLFWwindow* aWindow)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();

            std::string path = (PathManager::GetEngineAssetsPath() / "Editor/Layouts/").generic_string() + currentEditorLayout + ".ini";

            ImGuiIO& io = ImGui::GetIO();
            io.IniFilename = NULL;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            io.ConfigWindowsMoveFromTitleBarOnly = true;

            ImGui_ImplGlfw_InitForOpenGL(aWindow, true);
            ImGui_ImplOpenGL3_Init("#version 460");

            ImGui::LoadIniSettingsFromDisk(path.c_str());

            ImPlot::CreateContext();
        }

        inline void NewFrame()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();
        }

        inline void Render()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        // Tools
        inline void DrawComponentHeader(const char* compName, bool& isDrawn, const float values = 0, ImGuiStyleVar styleFlags = ImGuiStyleVar_FrameRounding)
        {
            ImGui::PushStyleVar(styleFlags, values);
            isDrawn = ImGui::CollapsingHeader(compName);
            ImGui::PopStyleVar();
        }

    }
}