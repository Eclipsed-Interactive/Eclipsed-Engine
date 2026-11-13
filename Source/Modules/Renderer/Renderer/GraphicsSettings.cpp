#include "GraphicsSettings.h"

#include <Core/Settings/SettingsRegistry.h>
#include <Core/Settings/SettingsBlock.h>

namespace Eclipse::Settings
{
	static SettingsBlock<GraphicsSettings> graphics_block{ GraphicsSettings::Name, SettingsScope::Project };

	GraphicsSettings& GetGraphicsSettings()
	{
		return graphics_block.Get();
	}
}