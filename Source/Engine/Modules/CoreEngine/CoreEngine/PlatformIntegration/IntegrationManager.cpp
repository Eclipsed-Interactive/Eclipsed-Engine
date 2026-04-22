#include "pch.h"

#include "IntegrationManager.h"

#include "DiscordIntegration.h"

namespace Eclipse::PlatformIntegration
{
	void IntegrationManager::Update()
	{
		Discord::Update();
	}
}