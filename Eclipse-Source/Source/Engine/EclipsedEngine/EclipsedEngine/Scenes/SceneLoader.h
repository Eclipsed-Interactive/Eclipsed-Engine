#pragma once

#include "rapidjson/document.h"

#include <string>

namespace Eclipse
{
	class Canvas;
	
	namespace Reflection
	{
		class AbstractSerializedVariable;
	}

	class SceneLoader
	{
	public:
		static void Save(const char* aPath);
		static void Load(const char* aPath);

		static void WriteMember(rapidjson::Value& aValue, Reflection::AbstractSerializedVariable* aSerializedVariable, rapidjson::Document::AllocatorType& alloc);
		static void LoadComponent(const std::string& componentName, const rapidjson::Value& aValue);

		struct ChildObject
		{
			GameObject* gameobject;
			unsigned ownerID;
		};

		static Canvas* GetParentCanvas(GameObject* BaseObject);
		
		static void LoadChildren(std::vector<ChildObject> aChildObjects);
		static void LoadType(Reflection::AbstractSerializedVariable* aSerializedVariable, const rapidjson::Value& aValue);
	private:

	};
}