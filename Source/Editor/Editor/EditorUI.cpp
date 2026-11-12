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


		// Text
		colors[ImGuiCol_Text] = ImVec4(0.863f, 0.867f, 0.882f, 1.00f); // #DCDDE1
		colors[ImGuiCol_TextDisabled] = ImVec4(0.420f, 0.427f, 0.463f, 1.00f); // #6B6D76
		colors[ImGuiCol_TextLink] = ImVec4(0.96f, 0.64f, 0.35f, 1.00f); // Corona rim
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.91f, 0.47f, 0.18f, 0.35f);
		colors[ImGuiCol_InputTextCursor] = ImVec4(0.96f, 0.64f, 0.35f, 1.00f);

		// Windows
		colors[ImGuiCol_WindowBg] = ImVec4(0.078f, 0.082f, 0.098f, 1.00f); // Umbra #141519
		colors[ImGuiCol_ChildBg] = ImVec4(0.067f, 0.071f, 0.086f, 1.00f); // Inset #111216
		colors[ImGuiCol_PopupBg] = ImVec4(0.102f, 0.106f, 0.125f, 0.98f);
		colors[ImGuiCol_Border] = ImVec4(0.149f, 0.153f, 0.176f, 1.00f); // Line #26272D
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.055f, 0.059f, 0.071f, 1.00f); // Deep space #0E0F12

		colors[ImGuiCol_TitleBg] = ImVec4(0.055f, 0.059f, 0.071f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.055f, 0.059f, 0.071f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.055f, 0.059f, 0.071f, 0.75f);

		// Frames
		colors[ImGuiCol_FrameBg] = ImVec4(0.102f, 0.106f, 0.125f, 1.00f); // Penumbra #1A1B20
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.129f, 0.133f, 0.157f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.165f, 0.169f, 0.196f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.91f, 0.47f, 0.18f, 1.00f); // Corona #E8772E
		colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.47f, 0.18f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.96f, 0.64f, 0.35f, 1.00f);

		// Buttons - neutral, the corona is saved for "on" / selected
		colors[ImGuiCol_Button] = ImVec4(0.129f, 0.133f, 0.157f, 1.00f); // #212228
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.165f, 0.169f, 0.196f, 1.00f); // #2A2B32
		colors[ImGuiCol_ButtonActive] = ImVec4(0.200f, 0.204f, 0.235f, 1.00f); // #33343C

		// Headers / selection - corona glow
		colors[ImGuiCol_Header] = ImVec4(0.91f, 0.47f, 0.18f, 0.18f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.47f, 0.18f, 0.26f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.91f, 0.47f, 0.18f, 0.36f);

		colors[ImGuiCol_Separator] = ImVec4(0.149f, 0.153f, 0.176f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.47f, 0.18f, 0.60f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.91f, 0.47f, 0.18f, 1.00f);

		colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.47f, 0.18f, 0.50f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.91f, 0.47f, 0.18f, 0.90f);

		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.165f, 0.169f, 0.196f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.200f, 0.204f, 0.235f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.91f, 0.47f, 0.18f, 0.80f);

		// Tabs - selected tab merges into its panel, corona line on top
		colors[ImGuiCol_Tab] = ImVec4(0.055f, 0.059f, 0.071f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.129f, 0.133f, 0.157f, 1.00f);
		colors[ImGuiCol_TabSelected] = ImVec4(0.078f, 0.082f, 0.098f, 1.00f);
		colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.91f, 0.47f, 0.18f, 1.00f);
		colors[ImGuiCol_TabDimmed] = ImVec4(0.055f, 0.059f, 0.071f, 1.00f);
		colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.078f, 0.082f, 0.098f, 1.00f);
		colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.420f, 0.427f, 0.463f, 0.60f);

		// Docking
		colors[ImGuiCol_DockingPreview] = ImVec4(0.91f, 0.47f, 0.18f, 0.40f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.055f, 0.059f, 0.071f, 1.00f);

		// Tables
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.102f, 0.106f, 0.125f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.149f, 0.153f, 0.176f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.118f, 0.122f, 0.141f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);
		colors[ImGuiCol_TreeLines] = ImVec4(0.149f, 0.153f, 0.176f, 1.00f);

		// Plots
		colors[ImGuiCol_PlotLines] = ImVec4(0.91f, 0.47f, 0.18f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.96f, 0.64f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.91f, 0.47f, 0.18f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.96f, 0.64f, 0.35f, 1.00f);

		// Drag & drop / nav / modals
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.96f, 0.64f, 0.35f, 1.00f);
		colors[ImGuiCol_DragDropTargetBg] = ImVec4(0.91f, 0.47f, 0.18f, 0.12f);
		colors[ImGuiCol_UnsavedMarker] = ImVec4(0.91f, 0.27f, 0.05f, 1.00f); // Prominence #E8450D
		colors[ImGuiCol_NavCursor] = ImVec4(0.91f, 0.47f, 0.18f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.96f, 0.64f, 0.35f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
	}
}