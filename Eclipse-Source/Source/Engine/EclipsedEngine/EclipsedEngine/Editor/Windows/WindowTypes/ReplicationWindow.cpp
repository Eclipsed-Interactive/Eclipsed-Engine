#include "ReplicationWindow.h"

#include "ImGui/imgui.h"

#include "EclipsedEngine/Replication/ReplicationManager.h"

namespace Eclipse::Editor
{
    void ReplicationWindow::Update()
    {
        DrawSceneEditor();
    }

    void ReplicationWindow::DrawSceneEditor()
    {
        if (ImGui::Button("Listen Server"))
        {
            
        }
    }
}