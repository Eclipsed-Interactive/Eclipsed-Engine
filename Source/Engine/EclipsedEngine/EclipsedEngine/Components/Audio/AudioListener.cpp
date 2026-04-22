#include "AudioListener.h"

#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"

#include "EclipsedEngine/Components/Transform2D.h"

namespace Eclipse
{
	void AudioListener::Awake()
	{
		gameObject->transform->AddFunctionToRunOnDirtyUpdate(this, [this]() {UpdatePositionalData();});
	}

	// TODO: Fix so it supports rotation and object velocity.
	void AudioListener::UpdatePositionalData()
	{
		FMOD::System* sys = MainSingleton::GetRaw<FMOD::System*>();

		Math::Vector2f ePos = gameObject->transform->GetPosition();
		FMOD_VECTOR pos = { ePos.x, ePos.y, 0.f };

		sys->set3DListenerAttributes(0, &pos, nullptr, nullptr, nullptr);
	}
}