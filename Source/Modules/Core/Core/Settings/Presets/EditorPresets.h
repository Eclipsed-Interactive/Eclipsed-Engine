#pragma once

#include "Core/Settings/Config.h"

namespace Eclipse::Configs
{
	class EditorPresets
	{
	public:
		static inline Config Default()
		{
			Config config;
			config.Add<bool>("openLastSceneOnStartup", false);

			return config;
		}
	};
}