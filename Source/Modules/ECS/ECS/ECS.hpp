#pragma once

#include "ComponentManager.h"

namespace Eclipse
{
	template <typename T>
	inline T* GetComponent(GameObjectID aGOID)
	{
		return ComponentManager::Get().GetComponent<T>(aGOID);
	}

	template <typename T>
	inline T* AddComponent(GameObjectID aGOID)
	{
		return ComponentManager::Get().AddComponent<T>(aGOID);
	}
}