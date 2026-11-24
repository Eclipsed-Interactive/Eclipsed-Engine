#include "SceneSerializer.h"

#include "Serialization/Serialization.hpp"

#include "ECS/ComponentManager.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

#include "Reflection/Runtime/ReflectionRegistry.h"

#include <fstream>
#include <vector>

namespace Eclipse
{
	namespace {
		std::string SaveObject(GameObject* aObject, bool pretty = true)
		{
			using namespace rapidjson;
			constexpr int VERSION = 0;

			StringBuffer buffer;
			PrettyWriter<StringBuffer> writer(buffer);

			writer.StartObject();
			const std::string guidStr = aObject->GetGUID().ToString();

			{ // Write all generic gameobject data
				writer.Key("id");	writer.String(guidStr.c_str());
				writer.Key("name"); writer.String(aObject->GetName().c_str());
				writer.Key("static"); writer.Bool(aObject->GetIsStatic());
				writer.Key("version"); writer.Int(VERSION);
			}

			writer.Key("components");
			{
				writer.StartArray();

				rapidjson::MemoryPoolAllocator<> alloc;

				for (Component* comp : aObject->GetAllComponents())
				{
					const std::string compName = comp->GetComponentName();
					const Reflection::TypeEntry& type = Reflection::ReflectionRegistry::GetType(compName);

					writer.StartObject();
					{
						writer.Key("name"); writer.String(type.name.c_str());
						for (auto& [name, field] : type.fields)
						{
							const void* fieldPtr = reinterpret_cast<const unsigned char*>(comp) + field.offset;
							rapidjson::Value val = Serialization::ToJson(field.type, fieldPtr, alloc);

							writer.Key(name.c_str());
							val.Accept(writer);
						}
					}
					writer.EndObject();
					alloc.Clear();
				}

				writer.EndArray();
			}

			writer.EndObject();

			return buffer.GetString();
		}
	}

	SerializedScene SceneSerializer::Serialize(GUID guid, bool pretty)
	{
		using namespace rapidjson;

		SerializedScene scene;

		constexpr int VERSION = 0;

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);

		writer.StartObject();
		writer.Key("id"); writer.String(guid.ToString().c_str());

		writer.Key("objects");
		writer.StartArray();
		for (auto& [falseid, gameobject] : ComponentManager::GetGameObjects())
		{
			writer.String(gameobject->GetGUID().ToString().c_str());
			scene.objects.push_back(SerializedObject{ .guid = gameobject->GetGUID(), .json = SaveObject(gameobject)});
		}
		writer.EndArray();
		writer.EndObject();

		scene.manifest = buffer.GetString();

		return scene;
	}
}
