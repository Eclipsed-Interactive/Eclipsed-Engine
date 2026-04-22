#pragma once

#include "EclipsedEngine/Components/Component.h"

namespace FMOD::Studio {
	class EventDescription;
	class EventInstance;
}

namespace Eclipse {
	class ECLIPSED_API AudioListener : public Component
	{
		BASE_SELECTION(AudioListener, 1)

	public:
		void Awake() override;
		
	private:
		void UpdatePositionalData();
	};
}