#pragma once

#include "EclipsedEngine/Components/Component.h"
#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"
namespace FMOD::Studio {
	class EventDescription;
	class EventInstance;
}

namespace Eclipse {
	class ECL_API AudioListener : public Component
	{
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