#include "SceneSerializer.h"

#include "Core/Settings/EditorSettings.h"
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
	void SceneSerializer::LoadScene(const std::string& scene)
	{
		if (scene.empty())
			return;

		rapidjson::Document d;
		d.Parse(scene.c_str());

		if (d.HasParseError())
		{
			throw std::runtime_error("Document couldnt be opened.");
		}

		if (d.HasMember("gameobjects") && d["gameobjects"].IsArray())
			LoadGameObjects(d["gameobjects"]);
		else return;

		if (d.HasMember("components") && d["components"].IsObject())
			LoadComponents(d["components"]);
	}

	void SceneSerializer::LoadScene(Assets::Scene scene)
	{
		if (scene.dataPtr->sourceBlob.empty())
			return;

		LoadScene(scene.dataPtr->sourceBlob);
	}

	void SceneSerializer::LoadGameObjects(const rapidjson::Value& gameobjects)
	{
		for (auto& go : gameobjects.GetArray())
		{
			unsigned int id = 0;
			std::string name = "";

			if (go.HasMember("name") && go["name"].IsString())
				name = go["name"].GetString();

			if (go.HasMember("id") && go["id"].IsUint())
				id = go["id"].GetUint();

			GameObject* obj = ComponentManager::CreateGameObject(id);
			obj->SetName(name);
		}
	}

	void SceneSerializer::LoadComponents(const rapidjson::Value& components)
	{
		for (const auto& typeMember : components.GetObject())
		{
			try
			{
				const Reflection::TypeEntry& reflectedType = Reflection::ReflectionRegistry::GetType(typeMember.name.GetString());
				for (const auto& comp : typeMember.value.GetArray())
				{
					unsigned int id = 0;
					unsigned int owner = 0;

					if (comp.HasMember("id") && comp["id"].IsUint())
						id = comp["id"].GetUint();

					if (comp.HasMember("owner") && comp["owner"].IsUint())
						owner = comp["owner"].GetUint();

					Component* Comp = ComponentManager::AddComponentWithID(owner, id, reflectedType.createAtAddress, reflectedType.size);

					Comp->SetComponentID(id);

					for (auto& member : comp["data"].GetObject())
					{
						const char* key = member.name.GetString();
						const rapidjson::Value& value = member.value;
						const Reflection::FieldEntry& field = Reflection::ReflectionRegistry::GetField(typeMember.name.GetString(), key);

						unsigned char* data = reinterpret_cast<unsigned char*>(Comp) + field.offset;

						Serialization::FromJson(field.type, reinterpret_cast<void*>(data), value);
					}
				}
			}
			catch (...)
			{
				// Component is not reflected. Cant be loaded.
			}
		}
	}
}
