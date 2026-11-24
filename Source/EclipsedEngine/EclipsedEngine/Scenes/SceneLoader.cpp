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
#include <sstream>

// Windows.h (included by other files in the unity batch) defines GetObject as a macro
#ifdef GetObject
#undef GetObject
#endif

namespace Eclipse
{
	namespace {
		void LoadComponents(GameObject* obj, const rapidjson::Value& components)
		{
			for (const auto& type : components.GetArray())
			{
				const Reflection::TypeEntry* reflectedType = nullptr;
				std::string typeStr = "";
				if (type.HasMember("component_type_str") && type["component_type_str"].IsString())
				{
					typeStr = type["component_type_str"].GetString();
					reflectedType = Reflection::ReflectionRegistry::TryGetType(typeStr);
				}

				if (!reflectedType) continue;

				unsigned int id = 0;
				unsigned int owner = 0;

				if (type.HasMember("id") && type["id"].IsUint())
					id = type["id"].GetUint();

				if (type.HasMember("owner") && type["owner"].IsUint())
					owner = type["owner"].GetUint();

				Component* Comp = ComponentManager::AddComponent(obj->GetID(), reflectedType->createAtAddress, reflectedType->size);

				for (auto& member : type["fields"].GetObject())
				{
					const char* key = member.name.GetString();
					const rapidjson::Value& value = member.value;
					const Reflection::FieldEntry& field = Reflection::ReflectionRegistry::GetField(typeStr, key);

					unsigned char* data = reinterpret_cast<unsigned char*>(Comp) + field.offset;

					Serialization::FromJson(field.type, reinterpret_cast<void*>(data), value);
				}
			}
		}

		void LoadGameObjects(const std::string& aName, const rapidjson::Value& gameobjects)
		{
			for (auto& go : gameobjects.GetArray())
			{
				const std::string guidStr = go.GetString();

				const std::filesystem::path path = PathManager::GetAssetsPath() / "__external__" / "Scenes" / aName / guidStr;

				std::ifstream file(path, std::ios::binary);
				std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

				rapidjson::Document d;
				d.Parse(json.c_str());

				GUID guid;  std::string name; bool isStatic = false;
				if (d.HasMember("id") && d["id"].IsString()) guid.FromString(d["id"].GetString());
				if (d.HasMember("name") && d["name"].IsString()) name = d["name"].GetString();
				if (d.HasMember("static") && d["static"].IsBool()) isStatic = d["static"].GetBool();
				GameObject* obj = guid.IsEmpty() ? ComponentManager::CreateGameObject() : ComponentManager::CreateGameObject(guid);

				obj->SetIsStatic(isStatic);
				obj->SetName(name);
			}


			for (auto& go : gameobjects.GetArray())
			{
				const std::string guidStr = go.GetString();

				const std::filesystem::path path = PathManager::GetAssetsPath() / "__external__" / "Scenes" / aName / guidStr;

				std::ifstream file(path, std::ios::binary);
				std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

				rapidjson::Document d;
				d.Parse(json.c_str());

				GUID guid;
				guid.FromString(guidStr);
				GameObject* obj = ComponentManager::GetGameObject(guid);

				if (d.HasMember("parent") && d["parent"].IsString())
				{
					GUID parentId;
					parentId.FromString(d["parent"].GetString());
					GameObject* parent = ComponentManager::GetGameObject(parentId);
					obj->SetParent(parent);
				}

				if (d.HasMember("components") && d["components"].IsArray())
					LoadComponents(obj, d["components"]);
			}

				
		}
	}

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

		std::string name = "Unknown Name";
		if (d.HasMember("name") && d["name"].IsString())
			name = d["name"].GetString();


		if (d.HasMember("objects") && d["objects"].IsArray())
			LoadGameObjects(name, d["objects"]);
	}

	void SceneSerializer::LoadScene(Scene scene)
	{
		if (scene.GetJson().empty())
			return;

		std::string str = std::string(scene.GetJson());
		LoadScene(str.c_str());
	}
}
