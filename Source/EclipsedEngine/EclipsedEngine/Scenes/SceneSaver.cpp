#include "SceneSerializer.h"

#include "Core/Standard/Serialization/Serialization.hpp"

#include "ECS/ComponentManager.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

#include "Reflection/Runtime/ReflectionRegistry.h"

namespace Eclipse
{
	std::string SceneSerializer::SaveScene(bool pretty)
	{
		rapidjson::Document d;
		d.SetObject();

		auto& alloc = d.GetAllocator();

		rapidjson::Value gameobjects(rapidjson::kArrayType);
		for (auto& [id, obj] : ComponentManager::GetGameObjects())
		{
			rapidjson::Value gameobject(rapidjson::kObjectType);

			gameobject.AddMember("id", obj->GetID(), alloc);

			const std::string& name = obj->GetName();
			gameobject.AddMember("name",
				rapidjson::Value(name.c_str(),
					name.size(),
					alloc),
				alloc);

			gameobjects.PushBack(gameobject, alloc);
		}

		d.AddMember("gameobjects", gameobjects.Move(), alloc);


		rapidjson::Value components(rapidjson::kObjectType);

		for (auto& compList : ComponentManager::GetAllComponents())
		{
			for (auto comp : compList.vector)
			{
				rapidjson::Value component(rapidjson::kObjectType);
				component.AddMember("id", comp->myInstanceComponentID, alloc);
				component.AddMember("owner", comp->gameObject->GetID(), alloc);

				const std::string& name = comp->GetComponentName();

				try
				{
					const Reflection::TypeEntry& type = Reflection::ReflectionRegistry::GetType(name);
					rapidjson::Value compData(rapidjson::kObjectType);
					for (auto& [name, type] : type.fields)
					{
						try
						{
							unsigned char* ptr = reinterpret_cast<unsigned char*>(comp) + type.offset;
							rapidjson::Value val = Serialization::ToJson(type.type, ptr, alloc);

							rapidjson::Value key(type.name.c_str(), type.name.size(), alloc);
							compData.AddMember(key, val.Move(), alloc);
						}
						catch (...)
						{
							// No serializer
						}
					}

					component.AddMember("data", compData.Move(), alloc);
				}
				catch (...)
				{
					// The type is not reflected.
				}

				auto it = components.FindMember(name.c_str());
				if (it == components.MemberEnd())
				{
					rapidjson::Value key(name.c_str(), static_cast<rapidjson::SizeType>(name.size()), alloc);
					components.AddMember(key, rapidjson::Value(rapidjson::kArrayType), alloc);
					it = components.FindMember(name.c_str());
				}

				it->value.PushBack(component, alloc);
			}
		}

		d.AddMember("components", components.Move(), alloc);

		rapidjson::StringBuffer buffer;

		if (pretty)
		{
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);
		}
		else
		{
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);
		}



		return buffer.GetString();
	}
}
