#pragma once

#include <EclipsedEngine/EclipsedEngine.h>

namespace Eclipse::Editor
{
	class Editor final
	{
	public:
		void EarlyInit();
		void Init();
		void Shutdown();

		void Update();

		void BeginFrame();
		void Render();
		void EndFrame();

		bool ShouldClose();
	};
}