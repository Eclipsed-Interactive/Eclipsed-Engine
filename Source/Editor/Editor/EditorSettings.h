#pragma once

#include <Core/Math/Vector/Vector2.h>
#include "cereal/cereal.hpp"

namespace Eclipse::Settings
{
	struct EditorSettings
	{
		static constexpr const char* Name = "editor";

		std::string lastActiveScene = "";
		bool openLastActiveSceneOnStartup = true;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(lastActiveScene));
			ar(CEREAL_NVP(openLastActiveSceneOnStartup));
		}
	};

	EditorSettings& GetEditorSettings();
}