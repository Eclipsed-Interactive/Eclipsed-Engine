#include "PhysicsSettings.h"

#include <Core/Settings/SettingsRegistry.h>
#include <Core/Settings/SettingsBlock.h>

namespace Eclipse::Settings
{
	static SettingsBlock<PhysicsSettings> physics_block{ PhysicsSettings::Name, SettingsScope::Project };

	PhysicsSettings& GetPhysicsSettings()
	{
		return physics_block.Get();
	}
}