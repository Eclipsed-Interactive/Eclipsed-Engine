#pragma once

#include "ImGui/imgui.h"

class ImFont;

namespace Eclipse::Editor
{
	class EditorUI final
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

	private:
		static void LoadFonts();
		static void SetupStyle();

	public:
		static ImFont* FontTiny;
		static ImFont* FontExtraSmall;
		static ImFont* FontSmall;
		static ImFont* FontMedium;
		static ImFont* FontLarge;
		static ImFont* FontExtraLarge;
	};
}