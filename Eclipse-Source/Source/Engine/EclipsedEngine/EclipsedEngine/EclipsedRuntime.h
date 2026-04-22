#pragma once

#include "CoreEngine/Engine.h"
#include "CoreEngine/Macros/defines.h"

#include <thread>

namespace Eclipse
{
	class ECLIPSED_API EclipsedRuntime
	{
	public:
#ifdef ECLIPSED_EDITOR
		void StartEngine(const std::string& path);
#else
		void StartEngine();
#endif

		void UpdateGame();

		void Render();

		void Update();
		void Shutdown();
		void EndFrame();
		bool BeginFrame();


	private:
		Engine engine;

		//std::thread renderThread;
	};
}
