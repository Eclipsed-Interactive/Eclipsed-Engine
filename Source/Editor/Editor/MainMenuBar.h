#pragma once

namespace Eclipse::Editor
{
	class MainMenuBar final
	{
	public:
		static void Init();
		static void Draw();

	private:
		static void DrawViews();
	};
}