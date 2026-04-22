#ifdef ECLIPSED_EDITOR
#include "EditorUIManager.h"

#include "ImGui/ImGui_Impl.h"
#include "EclipsedEngine/Editor/Common/TextureIconManager.h"
#include "CoreEngine/MainSingleton.h"

//----------- Glad Includes ------------
#include "glad/glad.h"

//----------- GLFW Includes ------------
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "KHR/khrplatform.h"

#include "Font-Awesome/7/IconsFontAwesome7.h"

namespace Eclipse::Editor
{
	void EditorUIManager::Init()
	{
		GLFWwindow* window = MainSingleton::GetInstance<GLFWwindow*>();
		ImGui_Impl::ImplementImGui(window);

		IconManager::LoadIconsFile();
		LoadFonts();

		myWindowManager.Begin();
	}

	void EditorUIManager::BeginFrame()
	{
		ImGui_Impl::NewFrame();
		ImGui::PushFont(FontSmall);
	}

	void EditorUIManager::Update()
	{
		myWindowManager.Update();
		ImGui::PopFont();
	}
	
	void EditorUIManager::Render()
	{
		ImGui_Impl::Render();
	}

	void EditorUIManager::Shutdown()
	{
		myWindowManager.End();
		ImGui::SaveIniSettingsToDisk("imgui.ini");

		ImGui_ImplOpenGL3_Shutdown(); 
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorUIManager::LoadFonts()
	{
		ImGuiIO& io = ImGui::GetIO();

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;

		std::string arial = (PathManager::GetEngineAssetsPath() / "Fonts/ARIAL.TTF").generic_string();
		std::string faSolid900 = (PathManager::GetEngineAssetsPath() / "Fonts/FontAwesome/7/fa-solid-900.otf").generic_string();
		std::string faBrands400 = (PathManager::GetEngineAssetsPath() / "Fonts/FontAwesome/7/fa-brands-400.otf").generic_string();
		std::string faRegular400 = (PathManager::GetEngineAssetsPath() / "Fonts/FontAwesome/7/fa-regular-400.otf").generic_string();

		FontTiny = io.Fonts->AddFontFromFileTTF(arial.c_str(), 12.0f);
		io.Fonts->AddFontFromFileTTF(faSolid900.c_str(), 12.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faBrands400.c_str(), 12.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faRegular400.c_str(), 12.0f, &icons_config, icons_ranges);

		FontExtraSmall = io.Fonts->AddFontFromFileTTF(arial.c_str(), 14.0f);
		io.Fonts->AddFontFromFileTTF(faSolid900.c_str(), 14.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faBrands400.c_str(), 14.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faRegular400.c_str(), 14.0f, &icons_config, icons_ranges);

		FontSmall = io.Fonts->AddFontFromFileTTF(arial.c_str(), 16.0f);
		io.Fonts->AddFontFromFileTTF(faSolid900.c_str(), 16.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faBrands400.c_str(), 16.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faRegular400.c_str(), 16.0f, &icons_config, icons_ranges);

		FontMedium = io.Fonts->AddFontFromFileTTF(arial.c_str(), 20.0f);
		io.Fonts->AddFontFromFileTTF(faSolid900.c_str(), 20.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faBrands400.c_str(), 20.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faRegular400.c_str(), 20.0f, &icons_config, icons_ranges);

		FontLarge = io.Fonts->AddFontFromFileTTF(arial.c_str(), 32.0f);
		io.Fonts->AddFontFromFileTTF(faSolid900.c_str(), 32.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faBrands400.c_str(), 32.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faRegular400.c_str(), 32.0f, &icons_config, icons_ranges);

		FontExtraLarge = io.Fonts->AddFontFromFileTTF(arial.c_str(), 70.0f);
		io.Fonts->AddFontFromFileTTF(faSolid900.c_str(), 70.f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faBrands400.c_str(), 70.f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF(faRegular400.c_str(), 70.f, &icons_config, icons_ranges);

		io.Fonts->Build();

		io.FontDefault = FontSmall;
	}
}
#endif