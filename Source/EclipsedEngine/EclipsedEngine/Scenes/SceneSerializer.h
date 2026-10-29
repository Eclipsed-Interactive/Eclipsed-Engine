#pragma once

#include "EclipsedEngine.Core.hpp"

#include "Assets/Assets/SceneAsset.h"
#include <string>
#include "rapidjson/document.h"

namespace Eclipse
{
	class SceneSerializer
	{
	public:
		static std::string SaveScene(bool pretty = false);
		static void LoadScene(Assets::Scene scene);
		static void LoadScene(const std::string& scene);

	private:
		static void LoadGameObjects(const rapidjson::Value& gameobjects);
		static void LoadComponents(const rapidjson::Value& components);
	};
}