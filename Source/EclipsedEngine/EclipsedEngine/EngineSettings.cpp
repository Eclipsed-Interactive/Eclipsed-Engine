#include "EngineSettings.h"

#include <Core/Settings/SettingsRegistry.h>
#include <Core/Settings/SettingsBlock.h>

namespace Eclipse::Settings
{
	static SettingsBlock<EngineSettings> engine_block{ EngineSettings::Name, SettingsScope::Project };

	EngineSettings& GetEngineSettings()
	{
		return engine_block.Get();
	}
}