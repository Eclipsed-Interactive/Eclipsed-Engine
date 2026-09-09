#include "EditorUI.h"

#include "Common/TextureIconManager.h"
#include "FontAwesome/7/IconsFontAwesome7.h"
#include "ImGui/imgui.h"

#include "Core/PathManager.h"

namespace Eclipse::Editor
{
	ImFont* EditorUI::FontTiny = nullptr;
	ImFont* EditorUI::FontExtraSmall = nullptr;
	ImFont* EditorUI::FontSmall = nullptr;
	ImFont* EditorUI::FontMedium = nullptr;
	ImFont* EditorUI::FontLarge = nullptr;
	ImFont* EditorUI::FontExtraLarge = nullptr;

	void EditorUI::Init()
	{
		TextureIconManager::LoadAllTextureIcons();

		LoadFonts();
		SetupStyle();
	}

	void EditorUI::Shutdown()
	{

	}

	void EditorUI::BeginFrame()
	{

	}
	
	void EditorUI::EndFrame()
	{

	}
	
	void EditorUI::LoadFonts()
	{
		ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;

		

		std::string arial = (PathManager::GetEngineAssetsPath() / "Fonts/ARIAL.TTF").generic_string();
		std::string faSolid900 = (PathManager::GetEngineAssetsPath() / "Fonts/FontAwesome/7/fa-solid-900.otf").generic_string();
		std::string faBrands400 = (PathManager::GetEngineAssetsPath() / "Fonts/FontAwesome/7/fa-brands-400.otf").generic_string();
		std::string faRegular400 = (PathManager::GetEngineAssetsPath() / "Fonts/FontAwesome/7/fa-regular-400.otf").generic_string();

		ImGuiIO& io = ImGui::GetIO();

		auto LoadFont = [&](int fontSize) -> ImFont*{
			ImFont* font = io.Fonts->AddFontFromFileTTF(
				arial.c_str(),
				fontSize
			);

			io.Fonts->AddFontFromFileTTF(faSolid900.c_str(), fontSize, &icons_config, icons_ranges);
			io.Fonts->AddFontFromFileTTF(faBrands400.c_str(), fontSize, &icons_config, icons_ranges);
			io.Fonts->AddFontFromFileTTF(faRegular400.c_str(), fontSize, &icons_config, icons_ranges);

			return font;
			};

		FontTiny = LoadFont(12);
		FontExtraSmall = LoadFont(14);
		FontSmall = LoadFont(16);
		FontMedium = LoadFont(20);
		FontLarge = LoadFont(32);

		io.Fonts->Build();

		io.FontDefault = FontSmall;
	}
	
	void EditorUI::SetupStyle()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			// Corners
			style.WindowRounding = 4.0f;
			style.ChildRounding = 4.0f;
			style.FrameRounding = 3.0f;
			style.PopupRounding = 3.0f;
			style.ScrollbarRounding = 3.0f;
			style.GrabRounding = 3.0f;
			style.TabRounding = 3.0f;
		}


		colors[ImGuiCol_Text] = ImVec4(1.00f, 0.95f, 0.70f, 1.00f); // Golden yellow
		colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.45f, 0.25f, 1.00f);

		colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f); // Deep eclipse black
		colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.08f, 0.05f, 0.95f);

		colors[ImGuiCol_Border] = ImVec4(1.00f, 0.70f, 0.20f, 0.40f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.12f, 0.08f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.60f, 0.20f, 0.60f); // Soft orange glow
		colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.70f, 0.25f, 0.80f); // Yellow-orange

		colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.07f, 0.05f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.95f, 0.60f, 0.20f, 1.00f); // Eclipse rim orange
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);

		colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.85f, 0.70f, 0.20f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.95f, 0.60f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.75f, 0.30f, 1.00f);

		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.80f, 0.25f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.70f, 0.30f, 1.00f);

		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.18f, 0.12f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.60f, 0.20f, 0.85f); // Orange hover
		colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.75f, 0.30f, 1.00f); // Warm yellow active

		colors[ImGuiCol_Header] = ImVec4(0.20f, 0.18f, 0.12f, 0.55f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.95f, 0.60f, 0.20f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.75f, 0.30f, 1.00f);

		colors[ImGuiCol_Separator] = ImVec4(0.95f, 0.75f, 0.30f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.95f, 0.60f, 0.20f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.75f, 0.30f, 1.00f);

		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.75f, 0.30f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.95f, 0.60f, 0.20f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.75f, 0.35f, 0.95f);

		colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.10f, 0.08f, 1.00f);  // Neutral dark
		colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.78f, 0.30f, 0.95f);  // Brighter yellow highlight
		colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.50f, 0.15f, 1.00f);  // Amber/orange active
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.07f, 0.07f, 0.97f);  // Faded background tab
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.15f, 0.12f, 1.00f);  // Slightly brighter but still muted

		colors[ImGuiCol_DockingPreview] = ImVec4(0.95f, 0.65f, 0.25f, 0.70f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.85f, 0.40f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.95f, 0.55f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.80f, 0.30f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95f, 0.55f, 0.20f, 1.00f);

		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.95f, 0.60f, 0.20f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.85f, 0.30f, 0.90f);

		colors[ImGuiCol_NavHighlight] = ImVec4(0.95f, 0.65f, 0.25f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.85f, 0.50f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.95f, 0.65f, 0.25f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.95f, 0.65f, 0.25f, 0.35f);
	}
}