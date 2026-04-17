#pragma once

#include "AssetData.h"
#include "CoreEngine/Math/Vector/Vector2.h"

namespace Eclipse
{
	class GameObject;

	struct PrefabData : public AssetData {
		GameObject* gameobject = nullptr;
		char* data;

		~PrefabData()
		{
			free(data);
		}
	};
}