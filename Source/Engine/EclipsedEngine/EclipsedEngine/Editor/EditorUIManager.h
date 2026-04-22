#ifdef ECLIPSED_EDITOR
#pragma once

#include "EclipsedEngine/Editor/Windows/WindowManager.h"

namespace Eclipse::Editor
{
	class EditorUIManager final
	{
	public:
		void Init();

		void BeginFrame();
		void Update();
		void Render();

		void Shutdown();

	private:
		void LoadFonts();

		WindowManager myWindowManager;

	public:
		static inline class ImFont* FontTiny = nullptr;
		static inline class ImFont* FontExtraSmall = nullptr;
		static inline class ImFont* FontSmall = nullptr;
		static inline class ImFont* FontMedium = nullptr;
		static inline class ImFont* FontLarge = nullptr;
		static inline class ImFont* FontExtraLarge = nullptr;
	};
}
#endif