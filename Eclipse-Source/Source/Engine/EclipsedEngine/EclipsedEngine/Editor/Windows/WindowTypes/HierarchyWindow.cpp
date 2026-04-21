#ifdef ECLIPSED_EDITOR
#include "HierarchyWindow.h"

#include "rapidjson/stringbuffer.h"
#include "ImGui/imgui.h"

#include "EntityEngine/ComponentManager.h"
#include "EntityEngine/GameObject.h"

#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/TextRenderer.h"
#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/UI/UIImage.h"
#include "EclipsedEngine/Components/UI/Canvas.h"
#include "EclipsedEngine/Components/UI/Button.h"

#include "Editor/Windows/WindowTypes/AssetWindow/AssetWindow.h"
#include "Editor/Windows/WindowTypes/InspectorWindow.h"
#include "Editor/Common/EditorActions.h"
#include "Editor/EditorUIManager.h"

#include "Font-Awesome/7/IconsFontAwesome7.h"

namespace Eclipse::Editor
{
    void HierarchyWindow::OpenParents(unsigned aParentID)
    {
        GameObject* gameobject = ComponentManager::GetGameObject(aParentID);
        if (!gameobject)
            return;
        
        GameObject* parent = gameobject->GetParent();
        if (!parent)
            return;
        
        gameobjectIdsThatAreOpen.emplace(parent->GetID());
        OpenParents(parent->GetID());
    }

    void HierarchyWindow::HierarchyButton(GameObject* aGameObject, float totalIndent)
    {
        unsigned id = aGameObject->GetID();
        bool goIsOpen = gameobjectIdsThatAreOpen.find(id) != gameobjectIdsThatAreOpen.end();

        ImGui::PushFont(Editor::EditorUIManager::FontExtraSmall);
        if (aGameObject->GetChildCount() > 0)
        {
            float cursorXpos = ImGui::GetCursorPosX() + totalIndent;

            ImGui::SetCursorPosX(cursorXpos - 2);

            if (goIsOpen)
            {
                ImGui::Text(ICON_FA_SORT_DOWN);
                ImGui::SameLine();
            }
            else
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                ImGui::Text(ICON_FA_PLAY);
                ImGui::SameLine();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            }

            ImGui::SetCursorPosX(cursorXpos - 6);
            if (ImGui::InvisibleButton(std::string("##InvisibleButtonForChilding" + std::to_string(id)).c_str(), ImVec2(20, 20)))
            {
                if (gameobjectIdsThatAreOpen.find(id) != gameobjectIdsThatAreOpen.end())
                    gameobjectIdsThatAreOpen.erase(id);
                else
                    gameobjectIdsThatAreOpen.emplace(id);
            }
            ImGui::SameLine();
        }

        if (!goIsOpen && aGameObject->GetChildCount() > 0)
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
        }

        if (id == CurrentGameObjectID)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));

        ImGui::SetCursorPosX(totalIndent + 24);
        std::string buttonName = std::string(aGameObject->GetName() + "##" + std::to_string(id));
        const char* itemName = aGameObject->GetName().c_str();
        ImVec2 textSize = ImGui::CalcTextSize(itemName);
        bool clickedButton = ImGui::Button(buttonName.c_str(), ImVec2(textSize.x + 10, 20));

        if (id == CurrentGameObjectID)
            ImGui::PopStyleColor();

        if (ImGui::IsItemHovered())
        {
            gameobjectrightclicked = true;

            if (ImGui::IsMouseReleased(1))
            {
                ImGui::OpenPopup("GameobjectHierarchyRightClicked");
                SelectedGameobjectID = id;
            }
        }

        if (clickedButton)
        {
            CurrentGameObjectID = id;
            InspectorWindow::SetActiveType(ActiveItemTypes_GameObject);
        }

        ImGui::PopFont();


        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("DND_Childing_Reordering", &id, sizeof(unsigned));

            ImGui::Text(aGameObject->GetName().c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_Childing_Reordering"))
            {
                IM_ASSERT(payload->DataSize == sizeof(unsigned));
                unsigned draggedEntityID = *(const unsigned*)payload->Data;

                auto it = ComponentManager::myEntityIdToEntity.find(draggedEntityID);
                if (it != ComponentManager::myEntityIdToEntity.end())
                {
                    AssignParentChildren(it->second, aGameObject);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (gameobjectIdsThatAreOpen.find(id) != gameobjectIdsThatAreOpen.end())
        {
            std::vector<GameObject*> children = aGameObject->GetChildren();
            totalIndent += 24.f;

            for (auto& child : children)
            {
                HierarchyButton(child, totalIndent);
            }
        }
    }

    bool IsNewParentMyChild(Eclipse::GameObject* aParent, Eclipse::GameObject* aChild)
    {
        auto& children = aChild->GetChildren();
        for (auto& child : children)
        {
            if (child->GetID() == aParent->GetID())
                return true;

            if (IsNewParentMyChild(aParent, child))
                return true;
        }

        return false;
    }

    bool HierarchyWindow::CheckCopomentType(GameObject* aChild, GameObject* aParent)
    {
        if (aParent->GetComponent<Transform2D>() && aChild->transform)
        {
            /* code */
        }

        return true;
    }

    void SetCanvasForChildren(Canvas* aCanvas, std::vector<GameObject*> aChildren)
    {
        for (auto& child : aChildren)
        {
            if (auto* recttransform = child->GetComponent<RectTransform>())
            {
                recttransform->myCanvas = aCanvas;
                if (child->GetChildCount())
                    SetCanvasForChildren(aCanvas, child->GetChildren());
            }
        }
    }

    Canvas* HierarchyWindow::GetParentCanvas(GameObject* BaseObject)
    {
        Canvas* canvas = BaseObject->GetComponent<Canvas>();
        if (!canvas && BaseObject->GetParent())
            canvas = GetParentCanvas(BaseObject->GetParent());

        return canvas;
    }

    void HierarchyWindow::AssignParentChildren(GameObject* aChild, GameObject* aParent)
    {
        if (IsNewParentMyChild(aParent, aChild))
            return;

        auto& oldParent = aChild->GetParent();
        if (auto& parent = oldParent)
        {
            if (oldParent->GetID() == aParent->GetID())
                return;

            auto& children = parent->GetChildren();
            size_t childIndex = aChild->GetChildIndex();

            for (int i = childIndex; i < children.size() - 1; i++)
            {
                children[i] = children[i + 1];
                children[i]->SetChildIndex(i);
            }

            children.pop_back();
            if (children.size() <= 0)
                gameobjectIdsThatAreOpen.erase(parent->GetID());
        }

        if (aChild->transform && aParent->transform)
        {
            Math::Vector2f worldPosition = aChild->transform->GetPosition();
            float worldRotation = aChild->transform->GetRotation() * Math::rad2Deg;
            Math::Vector2f worldScale = aChild->transform->GetScale();

            aChild->SetParent(aParent);

            Math::Matrix3x3f newParentWorldMatrix = Math::Matrix3x3f::CreateTranslation(aParent->transform->GetPosition());
            Math::Matrix3x3f ParentRotationMatrix = Math::Matrix3x3f::CreateRotation(-aParent->transform->GetRotation());

            Math::Vector3f localPosVec3(worldPosition.x, worldPosition.y, 1);

            localPosVec3 = localPosVec3 * newParentWorldMatrix.GetInverse();
            localPosVec3 = localPosVec3 * ParentRotationMatrix.GetInverse();

            aChild->transform->SetPosition({localPosVec3.x, localPosVec3.y});

            float parentWorldRotation = aParent->transform->GetRotation() * Math::rad2Deg;
            float localRotation = worldRotation - parentWorldRotation;
            aChild->transform->SetRotation(localRotation);

            Math::Vector2f parentWorldScale = aParent->transform->GetScale();
            Math::Vector2f localScale = {
                worldScale.x / parentWorldScale.x,
                worldScale.y / parentWorldScale.y
            };
            aChild->transform->SetScale(localScale);
        }
        else
        {
            aChild->SetParent(aParent);
        }

        if (auto* recttransform = aChild->GetComponent<RectTransform>())
        {
            if (recttransform->myCanvas = GetParentCanvas(aChild))
            {
                recttransform->myCanvas->canvasCameraTransform.PositionOffset = {0.f, 0.f};
                recttransform->myCanvas->canvasCameraTransform.Rotation = 0.f;
                recttransform->myCanvas->canvasCameraTransform.ScaleMultiplier = {1.f, 1.f};
            }
        }

        gameobjectIdsThatAreOpen.emplace(aParent->GetID());
    }

    void HierarchyWindow::CreatePrefab(unsigned aGameobjectID, std::filesystem::path aPath)
    {
        std::string GameObjectName = ComponentManager::GetGameObject(aGameobjectID)->GetName();

        rapidjson::StringBuffer objectJson = EditorActions::CopyObject(aGameobjectID, false);
        std::ofstream outputStream(aPath / (GameObjectName + ".eprf"));
        outputStream << objectJson.GetString();
    }

    void HierarchyWindow::Update()
    {
        if (ImGui::BeginPopup("GameobjectHierarchyRightClicked"))
        {
            if (ImGui::BeginMenu("Actions"))
            {
                if (ImGui::MenuItem("Make Prefab"))
                {
                    if (AssetWindow::ActivePath.has_extension())
                        AssetWindow::ActivePath.remove_filename();

                    CreatePrefab(SelectedGameobjectID, AssetWindow::ActivePath);
                }

                if (ImGui::MenuItem("Un Child"))
                {
                    GameObject* gameobject = ComponentManager::GetGameObject(SelectedGameobjectID);

                    Transform2D* transform = gameobject->transform;

                    Math::Vector2f globalPosition = transform->GetPosition();
                    float globalRotation = transform->GetRotation() * Math::rad2Deg;
                    Math::Vector2f globalScale = transform->GetScale();

                    gameobject->SetParent(nullptr);

                    transform->SetPosition(globalPosition);
                    transform->SetRotation(globalRotation);
                    transform->SetScale(globalScale);
                }

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }


        if (!gameobjectrightclicked && ImGui::BeginPopupContextWindow("##CTX_MENU_RIGHT_CLICK", ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::BeginMenu("Create new..."))
            {
                if (ImGui::MenuItem("New Sprite"))
                {
                    GameObject* obj = ComponentManager::CreateGameObject();
                    Transform2D* transform = obj->AddComponent<Transform2D>();
                    transform->SetScale(Math::Vector2f(20, 20));

                    obj->AddComponent<SpriteRenderer2D>();

                    obj->SetName("New Sprite");
                }
                if (ImGui::MenuItem("Empty GameObject"))
                {
                    GameObject* obj = ComponentManager::CreateGameObject();
                    obj->AddComponent<Transform2D>();

                    obj->SetName("New GameObject");
                }
                if (ImGui::BeginMenu("Special"))
                {
                    if (ImGui::MenuItem("No Transform Object"))
                    {
                        GameObject* obj = ComponentManager::CreateGameObject();

                        obj->SetName("New No Transform Object");
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("UI"))
                {
                    if (ImGui::MenuItem("Canvas"))
                    {
                        GameObject* obj = ComponentManager::CreateGameObject();
                        obj->AddComponent<Transform2D>();
                        obj->AddComponent<Canvas>();

                        obj->SetName("Canvas");
                    }
                    if (ImGui::MenuItem("Image"))
                    {
                        GameObject* obj = ComponentManager::CreateGameObject();
                        obj->AddComponent<RectTransform>();
                        obj->AddComponent<UIImage>();

                        obj->SetName("New Image");
                    }
                    if (ImGui::MenuItem("Text"))
                    {
                        GameObject* obj = ComponentManager::CreateGameObject();
                        obj->AddComponent<RectTransform>();
                        obj->AddComponent<TextRenderer>();

                        obj->SetName("New Text");
                    }
                    if (ImGui::MenuItem("Button"))
                    {
                        GameObject* obj = ComponentManager::CreateGameObject();
                        obj->AddComponent<RectTransform>();
                        obj->AddComponent<Button>();
                        obj->AddComponent<UIImage>();
                        obj->SetName("New Button");

                        GameObject* textObj = ComponentManager::CreateGameObject();
                        textObj->AddComponent<RectTransform>();
                        TextRenderer* rend = textObj->AddComponent<TextRenderer>();
                        rend->SetText("Button");

                        obj->AddChild(textObj);
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }

        gameobjectrightclicked = false;

        if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
        {
            unsigned currentObject = HierarchyWindow::CurrentGameObjectID;
            if (currentObject > 0)
            {
                if (ComponentManager::myEntityIdToEntity.find(currentObject) != ComponentManager::myEntityIdToEntity.end())
                {
                    GameObject* gameobject = ComponentManager::myEntityIdToEntity.at(currentObject);
                    RecursiveDeleteChildren(gameobject);

                    gameobject->Delete();
                    gameobjectIdsThatAreOpen.erase(currentObject);
                    HierarchyWindow::CurrentGameObjectID = 0;
                }
            }
        }


        for (const auto& [id, data] : ComponentManager::myEntityIdToEntity)
        {
            GameObject* parent = data->GetParent();
            if (parent)
                continue;

            HierarchyButton(data, 0.f);
        }
    }

    void HierarchyWindow::RecursiveDeleteChildren(GameObject*& aGameObject)
    {
        auto& children = aGameObject->GetChildren();
        for (auto& child : children)
            RecursiveDeleteChildren(child);

        if (!aGameObject->IsOwner())
            return;

        ComponentManager::Destroy(aGameObject->GetID());
    }
}
#endif
