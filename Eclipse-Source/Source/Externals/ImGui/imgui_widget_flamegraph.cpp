// The MIT License(MIT)
//
// Copyright(c) 2019 Sandy Carter
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <imgui_widget_flamegraph.h>

#include "imgui.h"
#include "imgui_internal.h"

// Add these member variables to your ImGuiWidgetFlameGraph class
struct ViewState {
    float zoom = 1.0f;
    float panX = 0.0f;
    float panStartX = 0.0f;
    float scaleMin = 0.0f;
    float scaleMax = 1.0f;
    bool isPanning = false;
    bool isZooming = false;
    ImVec2 lastMousePos;
} viewState;

void ImGuiWidgetFlameGraph::PlotFlame(const char* label,
    void (*values_getter)(float* start, float* end, ImU8* level, const char** caption, const void* data, int idx),
    const void* data, int values_count, int values_offset,
    const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Find the maximum depth
    ImU8 maxDepth = 0;
    for (int i = values_offset; i < values_count; ++i)
    {
        ImU8 depth;
        values_getter(nullptr, nullptr, &depth, nullptr, data, i);
        maxDepth = ImMax(maxDepth, depth);
    }

    const auto blockHeight = ImGui::GetTextLineHeight() + (style.FramePadding.y * 2);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    if (graph_size.x == 0.0f)
        graph_size.x = ImGui::CalcItemWidth();
    if (graph_size.y == 0.0f)
        graph_size.y = label_size.y + (style.FramePadding.y * 3) + blockHeight * (maxDepth + 1);

    const ImRect frame_bb(window->DC.CursorPos,
        ImVec2(window->DC.CursorPos.x + graph_size.x, window->DC.CursorPos.y + graph_size.y));

    const ImRect inner_bb(
        ImVec2(frame_bb.Min.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y),
        ImVec2(frame_bb.Max.x - style.FramePadding.x, frame_bb.Max.y - style.FramePadding.y));

    const ImRect total_bb(frame_bb.Min,
        ImVec2(frame_bb.Max.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
            frame_bb.Max.y));

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, 0, &frame_bb))
        return;

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = values_offset; i < values_count; i++)
        {
            float v_start, v_end;
            values_getter(&v_start, &v_end, nullptr, nullptr, data, i);
            if (v_start == v_start) // Check non-NaN values
                v_min = ImMin(v_min, v_start);
            if (v_end == v_end) // Check non-NaN values
                v_max = ImMax(v_max, v_end);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    // Store original scale for reset functionality
    static float originalScaleMin = scale_min;
    static float originalScaleMax = scale_max;
    static bool resetRequested = false;

    // Handle zoom and pan interactions
    ImGui::PushID(label);

    // Check if mouse is over the graph
    bool isHovered = ImGui::IsItemHovered();

    // Handle mouse wheel for zooming
    if (isHovered && !ImGui::IsAnyItemActive())
    {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f)
        {
            // Get mouse position relative to graph
            ImVec2 mousePos = ImGui::GetMousePos();
            float relativeX = (mousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x);

            // Calculate zoom center
            float zoomFactor = (wheel > 0) ? 1.1f : 0.9f;
            float newZoom = viewState.zoom * zoomFactor;
            newZoom = ImClamp(newZoom, 0.1f, 100.0f);

            if (newZoom != viewState.zoom)
            {
                // Adjust pan to keep zoom centered on mouse position
                float zoomDelta = newZoom - viewState.zoom;
                viewState.panX -= relativeX * zoomDelta / viewState.zoom;
                viewState.panX = ImClamp(viewState.panX, -newZoom + 1.0f, 0.0f);
                viewState.zoom = newZoom;
            }
        }

        // Handle middle mouse button or Ctrl+Left mouse for panning
        bool panTrigger = ImGui::IsMouseClicked(ImGuiMouseButton_Middle) ||
            (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::GetIO().KeyCtrl);
        bool panRelease = ImGui::IsMouseReleased(ImGuiMouseButton_Middle) ||
            (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::GetIO().KeyCtrl);

        if (panTrigger)
        {
            viewState.isPanning = true;
            viewState.panStartX = viewState.panX;
            viewState.lastMousePos = ImGui::GetMousePos();
        }

        if (viewState.isPanning && panRelease)
        {
            viewState.isPanning = false;
        }

        if (viewState.isPanning)
        {
            ImVec2 currentMousePos = ImGui::GetMousePos();
            float deltaX = (currentMousePos.x - viewState.lastMousePos.x) / (inner_bb.Max.x - inner_bb.Min.x);
            viewState.panX = ImClamp(viewState.panStartX + deltaX, -viewState.zoom + 1.0f, 0.0f);
        }

        // Handle double-click to reset view
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            resetRequested = true;
        }
    }

    // Reset view if requested
    if (resetRequested)
    {
        viewState.zoom = 1.0f;
        viewState.panX = 0.0f;
        resetRequested = false;
    }

    ImGui::PopID();

    // Calculate visible range based on zoom and pan
    float visibleStart = -viewState.panX / viewState.zoom;
    float visibleEnd = visibleStart + (1.0f / viewState.zoom);
    visibleStart = ImClamp(visibleStart, 0.0f, 1.0f);
    visibleEnd = ImClamp(visibleEnd, 0.0f, 1.0f);

    // Calculate displayed scale range
    float duration = scale_max - scale_min;
    float displayScaleMin = scale_min + visibleStart * duration;
    float displayScaleMax = scale_min + visibleEnd * duration;

    ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    // Add zoom/pan controls overlay
    if (isHovered)
    {
        ImGui::SetTooltip("Zoom: %.2fx\nPan: %.2f\nMiddle-click or Ctrl+Click to pan\nScroll to zoom\nDouble-click to reset",
            viewState.zoom, viewState.panX);
    }

    bool any_hovered = false;
    if (values_count - values_offset >= 1)
    {
        const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;
        const ImU32 col_hovered = ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x77FFFFFF;
        const ImU32 col_outline_base = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
        const ImU32 col_outline_hovered = ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x7FFFFFFF;

        for (int i = values_offset; i < values_count; ++i)
        {
            float stageStart, stageEnd;
            ImU8 depth;
            const char* caption;

            values_getter(&stageStart, &stageEnd, &depth, &caption, data, i);

            // Check if this block is within the visible range
            if (stageEnd < displayScaleMin || stageStart > displayScaleMax)
                continue;

            auto start = (stageStart - displayScaleMin) / (displayScaleMax - displayScaleMin);
            auto end = (stageEnd - displayScaleMin) / (displayScaleMax - displayScaleMin);

            // Clamp to visible range
            start = ImClamp(start, 0.0f, 1.0f);
            end = ImClamp(end, 0.0f, 1.0f);

            if (start >= end)
                continue;

            float width = inner_bb.Max.x - inner_bb.Min.x;
            float height = blockHeight * (maxDepth - depth + 1) - style.FramePadding.y;

            auto pos0 = ImVec2(inner_bb.Min.x + start * width,
                inner_bb.Min.y + height);
            auto pos1 = ImVec2(inner_bb.Min.x + end * width,
                inner_bb.Min.y + height + blockHeight);

            // Only render if visible
            if (pos0.x < inner_bb.Max.x && pos1.x > inner_bb.Min.x &&
                pos0.y < inner_bb.Max.y && pos1.y > inner_bb.Min.y)
            {
                bool v_hovered = false;
                if (ImGui::IsMouseHoveringRect(pos0, pos1))
                {
                    ImGui::SetTooltip("%s: %8.4g", caption, stageEnd - stageStart);
                    v_hovered = true;
                    any_hovered = true;
                }

                window->DrawList->AddRectFilled(pos0, pos1, v_hovered ? col_hovered : col_base);
                window->DrawList->AddRect(pos0, pos1, v_hovered ? col_outline_hovered : col_outline_base);

                // Only render text if there's enough space and it's within the visible area
                auto textSize = ImGui::CalcTextSize(caption);
                auto boxSize = ImVec2(pos1.x - pos0.x, pos1.y - pos0.y);
                if (textSize.x < boxSize.x && boxSize.x > 20.0f) // Minimum width for text
                {
                    ImVec2 textOffset = ImVec2((boxSize.x - textSize.x) * 0.5f, (boxSize.y - textSize.y) * 0.5f);
                    ImGui::RenderText(ImVec2(pos0.x + textOffset.x, pos0.y + textOffset.y), caption);
                }
            }
        }

        // Draw zoom indicators on the axes
        window->DrawList->AddLine(
            ImVec2(inner_bb.Min.x, inner_bb.Min.y + blockHeight * (maxDepth + 1)),
            ImVec2(inner_bb.Max.x, inner_bb.Min.y + blockHeight * (maxDepth + 1)),
            ImGui::GetColorU32(ImGuiCol_Text), 1.0f
        );

        // Text overlay
        if (overlay_text)
            ImGui::RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y),
                frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f, 0.0f));

        if (label_size.x > 0.0f)
            ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
    }

    if (!any_hovered && ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Visible Range: %8.4g - %8.4g\nTotal: %8.4g",
            displayScaleMin, displayScaleMax, scale_max - scale_min);
    }
}