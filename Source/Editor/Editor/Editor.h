#pragma once

namespace Eclipse::Editor
{
	class Editor final
	{
	public:
		void Init(void* imguiCtx);
		void Shutdown();

		void BeginFrame();
		void Render();
		void EndFrame();
	};
}