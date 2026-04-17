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

            for (int row = 0; row < sceneOrder.size(); row++)
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

            // ImGui::PushID(78347688345);
            // ImGui::TableHeadersRow();
            // ImGui::PopID();

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
                    if (ImGui::Checkbox("", &hasLayerBool))
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

        // std::string filePath = (PathManager::GetProjectRoot() / "Settings/CollisionLayers.json").generic_string();
        // rapidjson::Document document;
        // document.SetObject();
        // auto& allocator = document.GetAllocator();

        // rapidjson::Value layersArray(rapidjson::kArrayType);
        // for (int i = 0; i < layerCount; i++) {
        //     layersArray.PushBack(PhysicsEngine::myCollisionLayers[i], allocator);
        // }

        // document.AddMember("Layers", layersArray, allocator);

        // rapidjson::StringBuffer buffer;
        // rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        // document.Accept(writer);

        // std::ofstream ofs(filePath);
        // ofs << buffer.GetString();
        // ofs.close();
    }
}
#endif