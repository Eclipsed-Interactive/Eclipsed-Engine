#include "pch.h"

#include "Engine.h"

#include "PlatformIntegration/IntegrationManager.h"

#include "CoreEngine/Settings/Settings.h"
#include "CoreEngine/Profiler/PerformanceProfilerManager.h"

namespace Eclipse
{
	void Engine::Init()
	{
		EventSystem::Trigger("Engine-Load");
		Time::Init();

	}

	void Engine::Update()
	{
		PROFILE_SCOPED;

		PlatformIntegration::IntegrationManager::Update();

		Time::Update();
	}
	void Engine::End()
	{
		EventSystem::Trigger("Engine-Shutdown");
		MainSingleton::Destroy();
	}
}