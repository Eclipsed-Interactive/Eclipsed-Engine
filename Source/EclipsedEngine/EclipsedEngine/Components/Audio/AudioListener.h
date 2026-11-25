#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "EclipsedEngine.Core.hpp"
namespace FMOD::Studio {
	class EventDescription;
	class EventInstance;
}

namespace Eclipse {
	class ECL_API AudioListener : public Component
	{
		MAKE_REFLECTABLE(AudioListener);
		REFLECTION_FLAG(COMPONENT);

		BASE_SELECTION(AudioListener, 1)

	public:
		static AudioListener* GetListener();

	public:
		void Awake() override;
		
	private:
		void UpdatePositionalData();

		static AudioListener* ListenerInstance;
	};
}