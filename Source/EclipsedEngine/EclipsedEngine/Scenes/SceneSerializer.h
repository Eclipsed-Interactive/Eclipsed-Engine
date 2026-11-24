#pragma once

#include "EclipsedEngine.Core.hpp"

#include <AssetSystem/Assets/Types/Scene.h>
#include <string>
#include <vector>
#include "rapidjson/document.h"

namespace Eclipse
{
	struct SerializedObject
	{
		GUID guid;
		std::string json;
	};

	struct SerializedScene
	{
		std::string manifest;
		std::vector<SerializedObject> objects;
	};

	class GameObject;
	class SceneSerializer
	{
	public:
		static SerializedScene Serialize(GUID guid, bool pretty = false);

		static std::string SerializeObject(const GameObject* anObj, bool aIsPretty = false);

		static std::string SaveScene(bool pretty = false);
		static void LoadScene(Scene scene);
		static void LoadScene(const std::string& scene);
	};
}