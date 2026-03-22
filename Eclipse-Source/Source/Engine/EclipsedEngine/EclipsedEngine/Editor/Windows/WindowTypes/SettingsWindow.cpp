#ifdef ECLIPSED_EDITOR
#include "SettingsWindow.h"

#include "ImGui/imgui.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/PhysicsEngineSettings.h"

#include "CoreEngine/Math/Math.h"

#include <fstream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>

#include "EclipsedEngine/Scenes/SceneManager.h"

#include "EclipsedEngine/Editor/Common/DragAndDrop.h"
#include "CoreEngine/Files/FileInfo.h"
#include "CoreEngine/PathManager.h"

#include "CoreEngine/Settings/PhysicsSettings.h"

#include "ImGui/imgui_widget_flamegraph.h"

struct FlameData {
    struct Entry {
        float start, end;
        ImU8 level;
        const char* caption;
    };
    std::vector<Entry> entries;
};

// Getter function
void MyFlameGetter(float* start, float* end, ImU8* level, const char** caption, const void* data, int idx) {
    const FlameData* flameData = (const FlameData*)data;
    if (idx >= flameData->entries.size()) return;

    if (start) *start = flameData->entries[idx].start;
    if (end) *end = flameData->entries[idx].end;
    if (level) *level = flameData->entries[idx].level;
    if (caption) *caption = flameData->entries[idx].caption;
}

namespace Eclipse::Editor
{
    void GameSettingsWindow::Update()
    {
        static bool collisionLayersShown = false;
        collisionLayersShown = ImGui::CollapsingHeader("Collision Layers");

        if (collisionLayersShown) DrawCollisionLayerEditor();

        static bool sceneShown = false;
        sceneShown = ImGui::CollapsingHeader("Scene Build Settings");

        if (sceneShown) DrawSceneEditor();

        // Static test data - only initialized once
        static FlameData testData;
        static bool initialized = false;

        if (!initialized)
        {
            testData.entries.push_back({ 0.0f, 100.0f, 0, "main()" });
            testData.entries.push_back({ 10.0f, 30.0f, 1, "update()" });
            testData.entries.push_back({ 30.0f, 45.0f, 1, "render()" });
            testData.entries.push_back({ 45.0f, 60.0f, 1, "physics()" });
            testData.entries.push_back({ 15.0f, 25.0f, 2, "AI::think()" });
            testData.entries.push_back({ 25.0f, 28.0f, 2, "AI::decide()" });
            testData.entries.push_back({ 32.0f, 38.0f, 2, "draw::mesh()" });
            testData.entries.push_back({ 38.0f, 42.0f, 2, "draw::shader()" });
            testData.entries.push_back({ 48.0f, 55.0f, 2, "collision::detect()" });
            testData.entries.push_back({ 55.0f, 58.0f, 2, "collision::resolve()" });
            testData.entries.push_back({ 5.0f, 8.0f, 1, "init()" });
            testData.entries.push_back({ 60.0f, 75.0f, 1, "audio()" });
            testData.entries.push_back({ 62.0f, 68.0f, 2, "audio::load()" });
            testData.entries.push_back({ 68.0f, 73.0f, 2, "audio::play()" });
            testData.entries.push_back({ 75.0f, 95.0f, 1, "network()" });
            testData.entries.push_back({ 78.0f, 85.0f, 2, "network::send()" });
            testData.entries.push_back({ 85.0f, 92.0f, 40, "network::recv()" });
            initialized = true;
        }

        static bool showFlameGraph = true;
        if (ImGui::CollapsingHeader("Flame Graph Test"))
        {
            ImGuiWidgetFlameGraph::PlotFlame(
                "Test Flame Graph",
                MyFlameGetter,
                &testData,
                (int)testData.entries.size(),
                0,
                "Total: 100ms",
                0.0f,  // scale_min
                100.0f // scale_max
            );
        }
    }

    void GameSettingsWindow::DrawSceneEditor()
    {
        if (ImGui::Button("Save"))
        {
            SceneManager::SaveSceneData();
        }

        if (DragAndDrop::BeginTarget("Drag scene here", Utilities::FileInfo::FileType_Scene))
        {
            SceneManager::AddScene(DragAndDrop::payloadBuffer);
        }

        for (auto& [name, idx] : SceneManager::GetNameToIdx())
        {
            if (ImGui::Button(name.c_str()))
            {
                SceneManager::LoadScene(name);
            }
        }

        ImGui::Spacing();

        if (ImGui::BeginTable("SceneOrderTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, 10.0f);
            ImGui::TableSetupColumn("Scenes");
            ImGui::TableHeadersRow();

            static std::vector<std::string> sceneOrder;
            if (sceneOrder.empty() || sceneOrder.size() < SceneManager::GetNameToIdx().size())
            {
                sceneOrder.clear();

                for (auto& [name, _] : SceneManager::GetNameToIdx())
                {
                    sceneOrder.push_back(name);
                }
            }

            for (int row = 0; row < (int)sceneOrder.size(); row++)
            {
                const std::string& name = sceneOrder[row];

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text(std::to_string(row).c_str());

                ImGui::TableSetColumnIndex(1);

                ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("ROW_PAYLOAD", &row, sizeof(int));
                    ImGui::Text("Move %s", name.c_str());
                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ROW_PAYLOAD"))
                    {
                        std::vector<std::string>& paths = SceneManager::GetScenePaths();

                        int srcIndex = *(const int*)payload->Data;
                        if (srcIndex != row)
                        {
                            std::string movedPath = paths[srcIndex];
                            paths.erase(paths.begin() + srcIndex);

                            std::string moved = sceneOrder[srcIndex];
                            sceneOrder.erase(sceneOrder.begin() + srcIndex);

                            int insert_at = row;
                            if (srcIndex < row) insert_at--;

                            sceneOrder.insert(sceneOrder.begin() + insert_at, moved);
                            paths.insert(paths.begin() + insert_at, movedPath);

                            int idx = 0;
                            for (auto& path : sceneOrder)
                            {
                                SceneManager::GetNameToIdx()[path] = idx;
                                idx += 1;
                            }
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
            }

            ImGui::EndTable();
        }
    }

    void GameSettingsWindow::DrawCollisionLayerEditor()
    {
        ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;
        ImGuiTableFlags tableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit;

        int columnCount = layerCount;
        if (ImGui::BeginTable("CollisionLayers", columnCount + 2, tableFlags))
        {
            // Not visible without table headersrow
            ImGui::TableSetupColumn("Layers", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
            for (int i = columnCount - 1; i >= 0; i--)
                ImGui::TableSetupColumn(Layer::AsString(Layer(1 << i)).c_str(), columnFlags);

            ImGui::PushID(7834768345);
            ImGui::TableAngledHeadersRow();
            ImGui::PopID();

            int totalID = 0;
            for (int i = 0; i < columnCount; ++i)
            {
                totalID++;
                std::string collisionName = Layer::AsString(Layer(1 << i));

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", collisionName.c_str());

                for (int j = 0; j < columnCount - i; j++)
                {
                    ImGui::TableNextColumn();

                    totalID++;

                    uint64_t firstLayer = PhysicsEngine::myCollisionLayers[i];
                    int secondLayer = (1 << (layerCount - 1 - j));

                    int hasLayer = firstLayer & secondLayer;

                    bool hasLayerBool = static_cast<bool>(hasLayer);

                    ImGui::PushID(totalID);
                    ImGui::SetNextItemWidth(10);
                    if (ImGui::Checkbox("##collision", &hasLayerBool))
                    {
                        if (hasLayerBool)
                        {
                            PhysicsEngine::myCollisionLayers[layerCount - 1 - j] |= (1 << i);
                            PhysicsEngine::myCollisionLayers[i] |= (1 << (layerCount - 1 - j));
                        }
                        else
                        {
                            PhysicsEngine::myCollisionLayers[layerCount - 1 - j] &= ~(1 << i);
                            PhysicsEngine::myCollisionLayers[i] &= ~(1 << (layerCount - 1 - j));
                        }

                        SaveLayerEditToJSON();
                    }
                    ImGui::PopID();
                }
            }

            ImGui::EndTable();
        }
    }

    void GameSettingsWindow::SaveLayerEditToJSON()
    {
        Settings::PhysicsSettings::SetPhysicsLayers(PhysicsEngine::myCollisionLayers);
        Settings::PhysicsSettings::Save();
    }
}
#endif