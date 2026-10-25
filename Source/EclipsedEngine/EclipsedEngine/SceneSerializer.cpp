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
	void SceneSerializer::SaveScene()
	{
		const std::filesystem::path path = PathManager::GetAssetsPath() / "Default.scene";
		Settings::EditorSettings::SetLastActiveScene(path.generic_string());

		std::ofstream file(path, std::ios::binary);
		if (!file)
		{
			throw std::runtime_error("The file failed to open.");
		}

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
				component.AddMember("id", comp->GetID(), alloc);
				component.AddMember("owner", comp->gameObject->GetID(), alloc);

				const std::string& name = comp->GetComponentName();

				try 
				{
					const Reflection::TypeEntry& type = Reflection::ReflectionRegistry::GetType(name);
					for (auto& [name, type] : type.fields)
					{
						try
						{
							unsigned char* ptr = reinterpret_cast<unsigned char*>(comp) + type.offset;
							 rapidjson::Value val = Serialization::ToJson(type.type, ptr, alloc);

							rapidjson::Value key(type.name.c_str(), type.name.size(), alloc);
							component.AddMember(key, val.Move(), alloc);
						}
						catch (...)
						{
							// No serializer
						}
					}
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
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		d.Accept(writer);
		file << buffer.GetString();
		file.close();



		//	//for()
		//}
	}
}
