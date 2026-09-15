#pragma once

namespace Eclipse::Editor
{
	class InputSettingsPanel final
	{
	public:
		static void Draw();

	private:
		void DrawInputDebug();
		void DrawInputMapper();
	};
}