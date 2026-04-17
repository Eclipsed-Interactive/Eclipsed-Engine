#ifdef ECLIPSED_EDITOR

#include "DragAndDrop.h"
#include "ImGui/imgui.h"

namespace Eclipse::Editor
{
    bool DragAndDrop::BeginTarget(const char* text, const Utilities::FileInfo::FileType_& type, bool* textClicked, bool* arrowClicked)
    {
        TextBox(text, { 5, 5 }, { 0,0,0,0 }, { 255, 255,255, 1 }, text, textClicked, arrowClicked);

        bool recievedData = false;
        if (ImGui::BeginDragDropTarget())
        {
            const char* dndString = dnd_id[type];

            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dndString))
            {
                index = payload->DataSize;
                memcpy(payloadBuffer, payload->Data, payload->DataSize);

                recievedData = true;
            }
            ImGui::EndDragDropTarget();
        }

        return recievedData;
    }

    void DragAndDrop::BeginSource(const char* payload, const size_t& size, const Utilities::FileInfo& info)
    {
        if (DragAndDrop::dnd_id.find(info.type) != DragAndDrop::dnd_id.end())
        {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                size_t assetPayloadSize = size;

                memcpy(&payloadBuffer[0], payload, assetPayloadSize);
                std::memset(&payloadBuffer[assetPayloadSize], 0, 1);
                const char* dnd = DragAndDrop::dnd_id[info.type];

                index = assetPayloadSize;
                ImGui::SetDragDropPayload(dnd, payloadBuffer, assetPayloadSize);

                ImGui::Text(payload);
                ImGui::EndDragDropSource();
            }
        }
    }

    //bool DragAndDrop::TextBox(const char* text,
    //    Math::Vector2f padding,
    //    const Math::Color& bgColor,
    //    const Math::Color& borderColor,
    //    const char* id,
    //    bool* boxClicked,
    //    bool* dropDownClicked)
    //{
    //    ImGui::PushID(id);

    //    float availableWidth = ImGui::GetContentRegionAvail().x;
    //    float maxWidth = availableWidth > 50.0f ? availableWidth : 50.0f;

    //    const float boxHeight = ImGui::GetTextLineHeightWithSpacing() + padding.y * 2.0f;
    //    const float arrowWidth = boxHeight; // square area for arrow (Unity style)

    //    ImVec2 pos = ImGui::GetCursorScreenPos();
    //    ImVec2 boxSize(maxWidth, boxHeight);

    //    ImDrawList* drawList = ImGui::GetWindowDrawList();

    //    // --- Background ---
    //    ImU32 bgCol = ImGui::GetColorU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, 1.0f));
    //    drawList->AddRectFilled(pos,  ImVec2(pos.x + boxSize.x, pos.y + boxSize.y), bgCol);

    //    // --- Border ---
    //    ImU32 borderCol = ImGui::GetColorU32(ImVec4(borderColor.r, borderColor.g, borderColor.b, 1.0f));
    //    drawList->AddRect(pos, ImVec2(pos.x + boxSize.x, pos.y + boxSize.y), borderCol, 0.0f, 0, 1.5f);

    //    // --- Split into areas ---
    //    ImVec2 textAreaSize(boxSize.x - arrowWidth, boxSize.y);
    //    ImVec2 arrowAreaPos(pos.x + textAreaSize.x, pos.y);
    //    ImVec2 arrowAreaSize(arrowWidth, boxSize.y);

    //    // --- Text area ---
    //    ImGui::SetCursorScreenPos(pos);
    //    bool textClicked = ImGui::InvisibleButton("##TextArea", textAreaSize);
    //    bool textHovered = ImGui::IsItemHovered();

    //    // --- Text rendering ---
    //    ImVec2 textPos = ImVec2(pos.x + padding.x, pos.y + padding.y);
    //    ImVec2 clipMax = ImVec2(arrowAreaPos.x - padding.x, pos.y + boxSize.y - padding.y);

    //    drawList->PushClipRect(textPos, clipMax, true);
    //    drawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), text);
    //    drawList->PopClipRect();

    //    // --- Arrow area ---
    //    ImGui::SetCursorScreenPos(arrowAreaPos);
    //    bool arrowClicked = ImGui::InvisibleButton("##ArrowArea", arrowAreaSize);
    //    bool arrowHovered = ImGui::IsItemHovered();

    //    // Draw arrow background when hovered (optional)
    //    if (arrowHovered)
    //    {
    //        ImU32 hoverCol = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
    //        drawList->AddRectFilled(arrowAreaPos, ImVec2(arrowAreaPos.x + arrowAreaSize.x, arrowAreaPos.y + arrowAreaSize.y), hoverCol);
    //    }

    //    // --- Arrow icon ---
    //    ImVec2 center = ImVec2(
    //        arrowAreaPos.x + arrowAreaSize.x * 0.5f,
    //        arrowAreaPos.y + arrowAreaSize.y * 0.5f);
    //    const float r = boxHeight * 0.25f;

    //    ImVec2 a(center.x - r, center.y - r * 0.5f);
    //    ImVec2 b(center.x + r, center.y - r * 0.5f);
    //    ImVec2 c(center.x, center.y + r * 0.5f);

    //    drawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));

    //    // --- Hover highlight like Unity (optional) ---
    //    if (textHovered)
    //    {
    //        ImU32 highlightCol = ImGui::GetColorU32(ImVec4(1, 1, 1, 0.1f));
    //        drawList->AddRectFilled(pos, ImVec2(pos.x + textAreaSize.x, pos.y + textAreaSize.y), highlightCol);
    //    }

    //    // --- Advance cursor below the box ---
    //    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + boxSize.y));

    //    ImGui::PopID();

    //    if (boxClicked) *boxClicked = textClicked;
    //    if (dropDownClicked) *dropDownClicked = arrowClicked;

    //    return textClicked || arrowClicked;






        //ImGui::PushID(id);

        //float availableWidth = ImGui::GetContentRegionAvail().x;
        //float maxWidth = availableWidth > 50.0f ? availableWidth : 50.0f;

        //// Fixed height for your box
        //const float boxHeight = ImGui::GetTextLineHeightWithSpacing() + padding.y * 2.0f;

        //// Calculate box size
        //ImVec2 boxSize(maxWidth, boxHeight);
        //ImVec2 pos = ImGui::GetCursorScreenPos();

        //// Draw background
        //ImGui::GetWindowDrawList()->AddRectFilled(
        //    pos, ImVec2(pos.x + boxSize.x, pos.y + boxSize.y),
        //    ImGui::GetColorU32({ aBgColor.r, aBgColor.g, aBgColor.b, 1.f }));

        //// Draw border
        //ImGui::GetWindowDrawList()->AddRect(
        //    pos, ImVec2(pos.x + boxSize.x, pos.y + boxSize.y),
        //    ImGui::GetColorU32({ aBorderColor.r, aBorderColor.g, aBorderColor.b, 1.f }),
        //    0.0f, ImDrawFlags_RoundCornersNone, 1.5f);

        //// Hit box
        //ImGui::SetCursorScreenPos(pos);
        //bool clicked = ImGui::InvisibleButton("##hitbox", boxSize);

        //// Clip text so it doesn�t overflow the box
        //ImVec2 textPos(pos.x + padding.x, pos.y + padding.y);
        //ImVec2 clipMax(pos.x + boxSize.x - padding.x, pos.y + boxSize.y - padding.y);

        //ImGui::GetWindowDrawList()->PushClipRect(textPos, clipMax, true);
        //ImGui::GetWindowDrawList()->AddText(
        //    textPos,
        //    ImGui::GetColorU32(ImGuiCol_Text),
        //    text);
        //ImGui::GetWindowDrawList()->PopClipRect();

        //// Advance cursor below box
        //ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + boxSize.y));

        //ImGui::PopID();

        //return clicked;

    //}
    // 
    // 
    // 
    // 
    // 












    // 
    // 
//bool DragAndDrop::TextBox(const char* text,
//    Math::Vector2f padding,
//    const Math::Color& bgColor,
//    const Math::Color& borderColor,
//    const char* id,
//    bool* boxClicked,
//    bool* dropDownClicked)
//{
//    ImGui::PushID(id);
//
//    float availableWidth = ImGui::GetContentRegionAvail().x;
//    float maxWidth = availableWidth > 50.0f ? availableWidth : 50.0f;
//
//    const float boxHeight = ImGui::GetTextLineHeightWithSpacing() + padding.y * 2.0f;
//    const float arrowWidth = boxHeight; // square area for arrow (Unity style)
//
//    ImVec2 pos = ImGui::GetCursorScreenPos();
//    ImVec2 boxSize(maxWidth, boxHeight);
//
//    ImDrawList* drawList = ImGui::GetWindowDrawList();
//
//    // --- Background ---
//    ImU32 bgCol = ImGui::GetColorU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, 1.0f));
//    drawList->AddRectFilled(pos, ImVec2(pos.x + boxSize.x, pos.y + boxSize.y), bgCol);
//
//    // --- Border ---
//    ImU32 borderCol = ImGui::GetColorU32(ImVec4(borderColor.r, borderColor.g, borderColor.b, 1.0f));
//    drawList->AddRect(pos, ImVec2(pos.x + boxSize.x, pos.y + boxSize.y), borderCol, 0.0f, 0, 1.5f);
//
//    // --- Split into areas ---
//    ImVec2 textAreaSize(boxSize.x - arrowWidth, boxSize.y);
//    ImVec2 arrowAreaPos(pos.x + textAreaSize.x, pos.y);
//    ImVec2 arrowAreaSize(arrowWidth, boxSize.y);
//
//    // --- Entire box is now an InvisibleButton --- 
//    bool boxClickedResult = ImGui::InvisibleButton("##BoxArea", boxSize); // Entire box is a clickable area
//    bool boxHovered = ImGui::IsItemHovered();
//
//    // --- Text area (without affecting drag/drop region) ---
//    ImVec2 textPos = ImVec2(pos.x + padding.x, pos.y + padding.y);
//    ImVec2 clipMax = ImVec2(arrowAreaPos.x - padding.x, pos.y + boxSize.y - padding.y);
//
//    drawList->PushClipRect(textPos, clipMax, true);
//    drawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), text);
//    drawList->PopClipRect();
//
//    // --- Arrow area (still interactive) ---
//    //ImGui::SetItemAllowOverlap();
//    ImGui::SetCursorScreenPos(arrowAreaPos);
//    bool arrowClickedResult = false;
//
//    if (ImGui::InvisibleButton("##ArrowArea", arrowAreaSize))
//    {
//        int i = 0;
//        i += 9284;
//        i += 9284;
//        i += 9284;
//        i += 9284;
//        i += 9284;
//    }
//    bool arrowHovered = ImGui::IsItemHovered();
//    
//    // Draw arrow background when hovered (optional)
//    if (arrowHovered)
//    {
//        ImU32 hoverCol = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
//        drawList->AddRectFilled(arrowAreaPos, ImVec2(arrowAreaPos.x + arrowAreaSize.x, arrowAreaPos.y + arrowAreaSize.y), hoverCol);
//    }
//
//    // --- Arrow icon ---
//    ImVec2 center = ImVec2(
//        arrowAreaPos.x + arrowAreaSize.x * 0.5f,
//        arrowAreaPos.y + arrowAreaSize.y * 0.5f);
//    const float r = boxHeight * 0.25f;
//
//    ImVec2 a(center.x - r, center.y - r * 0.5f);
//    ImVec2 b(center.x + r, center.y - r * 0.5f);
//    ImVec2 c(center.x, center.y + r * 0.5f);
//
//    drawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));
//
//    // --- Hover highlight like Unity (optional) ---
//    if (boxHovered)
//    {
//        ImU32 highlightCol = ImGui::GetColorU32(ImVec4(1, 1, 1, 0.1f));
//        drawList->AddRectFilled(pos, ImVec2(pos.x + textAreaSize.x, pos.y + textAreaSize.y), highlightCol);
//    }
//
//    // --- Advance cursor below the box ---
//    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + boxSize.y));
//
//    // **Now set up drag-and-drop for the entire box**
//
//
//    ImGui::PopID();
//
//    if (boxClicked) *boxClicked = boxClickedResult;
//    if (dropDownClicked) *dropDownClicked = arrowClickedResult;
//
//    return boxClickedResult || arrowClickedResult;
//}




bool DragAndDrop::TextBox(
    const char* text,
    Math::Vector2f padding,
    const Math::Color& bgColor,
    const Math::Color& borderColor,
    const char* id,
    bool* boxClicked,
    bool* dropDownClicked)
{
    ImGui::PushID(id);

    float availableWidth = ImGui::GetContentRegionAvail().x;
    float maxWidth = availableWidth > 50.0f ? availableWidth : 50.0f;

    const float boxHeight = ImGui::GetTextLineHeightWithSpacing() + padding.y * 2.0f;
    const float arrowWidth = boxHeight; // square area for arrow (Unity style)

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 boxSize(maxWidth, boxHeight);

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // --- Background ---
    ImU32 bgCol = ImGui::GetColorU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, 1.0f));
    drawList->AddRectFilled(pos, ImVec2(pos.x + boxSize.x, pos.y + boxSize.y), bgCol);

    // --- Border ---
    ImU32 borderCol = ImGui::GetColorU32(ImVec4(borderColor.r, borderColor.g, borderColor.b, 1.0f));
    drawList->AddRect(pos, ImVec2(pos.x + boxSize.x, pos.y + boxSize.y), borderCol, 0.0f, 0, 1.5f);

    // --- Split into areas ---
    ImVec2 textAreaSize(boxSize.x - arrowWidth, boxSize.y);
    ImVec2 arrowAreaPos(pos.x + textAreaSize.x, pos.y);
    ImVec2 arrowAreaSize(arrowWidth, boxSize.y);

    // ===============================================================
    // TEXT AREA BUTTON
    // ===============================================================
    ImGui::SetCursorScreenPos(pos);
    bool textClickedResult = ImGui::InvisibleButton("##TextArea", textAreaSize);
    bool textHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    // --- Render Text ---
    ImVec2 textPos = ImVec2(pos.x + padding.x, pos.y + padding.y);
    ImVec2 clipMax = ImVec2(arrowAreaPos.x - padding.x, pos.y + boxSize.y - padding.y);

    drawList->PushClipRect(textPos, clipMax, true);
    drawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), text);
    drawList->PopClipRect();

    // ===============================================================
    // ARROW AREA BUTTON
    // ===============================================================
    ImGui::SetCursorScreenPos(arrowAreaPos);
    ImGui::PushItemFlag(ImGuiSelectableFlags_AllowOverlap, true);
    bool arrowClickedResult = ImGui::InvisibleButton("##ArrowArea", arrowAreaSize);
    ImGui::PopItemFlag();

    bool arrowHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    // --- Draw arrow hover highlight ---
    if (arrowHovered)
    {
        ImU32 hoverCol = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
        drawList->AddRectFilled(arrowAreaPos,
            ImVec2(arrowAreaPos.x + arrowAreaSize.x, arrowAreaPos.y + arrowAreaSize.y),
            hoverCol);
    }

    // --- Draw arrow icon ---
    ImVec2 center = ImVec2(
        arrowAreaPos.x + arrowAreaSize.x * 0.5f,
        arrowAreaPos.y + arrowAreaSize.y * 0.5f);
    const float r = boxHeight * 0.25f;

    ImVec2 a(center.x - r, center.y - r * 0.5f);
    ImVec2 b(center.x + r, center.y - r * 0.5f);
    ImVec2 c(center.x, center.y + r * 0.5f);

    drawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));

    // ===============================================================
    // HOVER EFFECTS (Unity-style)
    // ===============================================================
    if (textHovered)
    {
        ImU32 highlightCol = ImGui::GetColorU32(ImVec4(1, 1, 1, 0.1f));
        drawList->AddRectFilled(pos,
            ImVec2(pos.x + textAreaSize.x, pos.y + textAreaSize.y),
            highlightCol);
    }

    // ===============================================================
    // ADVANCE CURSOR BELOW BOX
    // ===============================================================
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + boxSize.y));

    ImGui::PopID();

    // ===============================================================
    // OUTPUT RESULTS
    // ===============================================================
    if (boxClicked) *boxClicked = textClickedResult;
    if (dropDownClicked) *dropDownClicked = arrowClickedResult;

    return textClickedResult || arrowClickedResult;
}

}

#endif