#include "EditorSettings.h"

#include <Core/Settings/SettingsRegistry.h>
#include <Core/Settings/SettingsBlock.h>

namespace Eclipse::Settings
{
	static SettingsBlock<EditorSettings> editor_block{ EditorSettings::Name, SettingsScope::User };

	EditorSettings& GetEditorSettings()
	{
		return editor_block.Get();
	}
}