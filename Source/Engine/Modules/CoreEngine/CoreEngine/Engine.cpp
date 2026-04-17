#include "pch.h"

#include "Engine.h"

#include "PlatformIntegration/IntegrationManager.h"

#include "CoreEngine/Settings/Settings.h"


namespace Eclipse
{
	void Engine::Init()
	{
		EventSystem::Trigger("Engine-Load");
		Time::Init();

	}

	void Engine::Update()
	{
		PlatformIntegration::IntegrationManager::Update();

		Time::Update();
	}
	void Engine::End()
	{
		EventSystem::Trigger("Engine-Shutdown");
		MainSingleton::Destroy();
	}
}