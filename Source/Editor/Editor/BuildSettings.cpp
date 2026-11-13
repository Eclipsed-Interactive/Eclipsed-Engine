#include "BuildSettings.h"

#include <Core/Settings/SettingsRegistry.h>
#include <Core/Settings/SettingsBlock.h>

namespace Eclipse::Settings
{
	static SettingsBlock<BuildSettings> build_block{ BuildSettings::Name, SettingsScope::User };

	BuildSettings& GetBuildSettings()
	{
		return build_block.Get();
	}
}