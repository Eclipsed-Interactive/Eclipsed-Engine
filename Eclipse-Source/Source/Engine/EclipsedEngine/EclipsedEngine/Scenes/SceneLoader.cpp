#include "SceneLoader.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

#include <fstream>

#include "EclipsedEngine/Reflection/Reflection.h"
#include "EclipsedEngine/Reflection/SerializedVariable.h"
#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"

#include "CoreEngine/ChatGPT_Dump/Base64.hpp"

#include "EntityEngine/Component.h"
#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/Canvas.h"

#include "GraphicsEngine/RenderCommands/CommandList.h"

#include "PhysicsEngine/PhysicsEngine.h"

#include "CoreEngine/Debug/DebugLogger.h"

#include "AssetEngine/Resources.h"

namespace Eclipse
{
	void SceneLoader::Save(const char* aPath)
	{
		rapidjson::Document d;
		d.SetObject();

		rapidjson::Document::AllocatorType& alloc = d.GetAllocator();

		std::unordered_map<std::string, std::unordered_map<unsigned, rapidjson::Value>> components;

		for (Component* pComp : ComponentManager::myComponents)
		{
			std::string compName = pComp->GetComponentName();

			if (compName == "Component")
			{
				continue;
			}
			rapidjson::Value value(rapidjson::kObjectType);

			value.AddMember("owner", pComp->gameObject->GetID(), alloc);
			value.AddMember("id", pComp->myInstanceComponentID, alloc);

			value.AddMember("IsReplicated", pComp->IsReplicated, alloc);

			components[compName][pComp->myInstanceComponentID] = value;
		}


		auto& list = Reflection::ReflectionManager::GetList();
		for (auto& [pComp, varList] : list)
		{
			for (auto& var : varList)
			{
				std::string compName = var->GetComponent()->GetComponentName();
				Component* pComp = var->GetComponent();
				rapidjson::Value& val = components[compName][pComp->myInstanceComponentID];

				WriteMember(val, var, alloc);
			}
		}

		for (auto& [name, list] : components)
		{
			rapidjson::Value compArray(rapidjson::kArrayType);
			for (auto& [id, compJson] : list)
			{
				compArray.PushBack(compJson, alloc);
			}

			rapidjson::Value nameKey = rapidjson::Value(name.c_str(), alloc);
			d.AddMember(nameKey.Move(), compArray.Move(), alloc);
		}

		rapidjson::Value goArray(rapidjson::kArrayType);
		for (auto& [id, gameobject] : ComponentManager::myEntityIdToEntity)
		{
			rapidjson::Value goObj(rapidjson::kObjectType);

			if (GameObject* parent = gameobject->GetParent())
				goObj.AddMember("owner", parent->GetID(), alloc);

			goObj.AddMember("id", id, alloc);
			goObj.AddMember("name", rapidjson::Value(gameobject->GetName().c_str(), alloc).Move(), alloc);
			goArray.PushBack(goObj, alloc);
		}
		d.AddMember("GameObjects", goArray, alloc);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		d.Accept(writer);

		std::ofstream ofs(aPath);
		ofs << buffer.GetString();
		ofs.close();
	}

	void SceneLoader::WriteMember(rapidjson::Value& aValue, Reflection::AbstractSerializedVariable* aSerialized, rapidjson::Document::AllocatorType& alloc)
	{
		using namespace rapidjson;

		aSerialized->ResolveTypeInfo();

		Value key(aSerialized->GetName(), alloc);
		Value jsonVal(kObjectType);

		if (aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_String)
		{
			void* rawPtr = aSerialized->GetData();
			std::string* strPtr = static_cast<std::string*>(rawPtr);
			std::string& encoded = *strPtr;
			jsonVal.SetString(encoded.c_str(), alloc);
		}


		else if (aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_List)
		{
			jsonVal.AddMember("size", aSerialized->GetCount(), alloc);

			void* rawPtr = aSerialized->GetData();
			std::string encoded = Base64::Encode(aSerialized->GetData(), aSerialized->GetSizeInBytes());
			jsonVal.AddMember("data", Value(encoded.c_str(), alloc).Move(), alloc);
		}

		else if (
			aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Prefab ||
			aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Texture ||
			aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Material ||
			aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_AudioClip
			)
		{
			std::string id = "";
			if (aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Texture)
			{
				Reflection::SerializedVariable<Texture>* asset = (Reflection::SerializedVariable<Texture>*)aSerialized;
				if (!asset->Get().IsValid()) return;
				id = asset->Get().GetAssetID();
			}
			else if (aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Material)
			{
				Reflection::SerializedVariable<Material>* asset = (Reflection::SerializedVariable<Material>*)aSerialized;
				if (!asset->Get().IsValid()) return;
				id = asset->Get().GetAssetID();
			}
			else if (aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_AudioClip)
			{
				Reflection::SerializedVariable<AudioClip>* asset = (Reflection::SerializedVariable<AudioClip>*)aSerialized;
				if (!asset->Get().IsValid()) return;
				id = asset->Get().GetAssetID();
			}
			else if (aSerialized->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Prefab)
			{
				Reflection::SerializedVariable<Prefab>* asset = (Reflection::SerializedVariable<Prefab>*)aSerialized;
				if (!asset->Get().IsValid()) return;
				id = asset->Get().GetAssetID();
			}

			jsonVal.SetString(id.c_str(), alloc);
		}

		else
		{
			int i = aSerialized->GetSizeInBytes();
			std::string encoded = Base64::Encode(aSerialized->GetData(), i);
			jsonVal.SetString(encoded.c_str(), alloc);
		}

		aValue.AddMember(key, jsonVal.Move(), alloc);
	}

	Canvas* SceneLoader::GetParentCanvas(GameObject* BaseObject)
	{
		Canvas* canvas = BaseObject->GetComponent<Canvas>();
		if (BaseObject->GetParent())
			canvas = GetParentCanvas(BaseObject->GetParent());
		
		return canvas;
	}

	void SceneLoader::LoadChildren(std::vector<ChildObject> aChildObjects)
	{
		for (ChildObject& child : aChildObjects)
		{
			GameObject* parent = ComponentManager::myEntityIdToEntity.at(child.ownerID);
			child.gameobject->SetParent(parent);

			if (auto* recttransform = child.gameobject->GetComponent<RectTransform>())
			{
				if (recttransform->myCanvas = GetParentCanvas(child.gameobject))
				{
					recttransform->myCanvas->canvasCameraTransform.PositionOffset = { 0.f, 0.f };
					recttransform->myCanvas->canvasCameraTransform.Rotation = 0.f;
					recttransform->myCanvas->canvasCameraTransform.ScaleMultiplier = { 1.f, 1.f };
				}
			}
		}
	}

	void SceneLoader::Load(const char* aPath)
	{
		using namespace rapidjson;

		CommandListManager::ResetAllCommandLists();

		ComponentManager::Clear();
		Reflection::ReflectionManager::ClearList();
		Replication::ReplicationManager::ClearList();

		PhysicsEngine::CleanUp();
		PhysicsEngine::InitWorld();

		std::ifstream ifs(aPath);
		if (!ifs.is_open()) {
			return;
		}

		std::string jsonString((std::istreambuf_iterator<char>(ifs)),
			std::istreambuf_iterator<char>());

		ifs.close();

		Document d;
		if (d.Parse(jsonString.c_str()).HasParseError()) return;

		if (!d.HasMember("GameObjects")) return;


		std::vector<ChildObject> childObjects;

		const Value& objs = d["GameObjects"];
		{
			for (const Value& obj : objs.GetArray())
			{
				unsigned int id = obj["id"].GetUint();
				std::string name = obj["name"].GetString();

				GameObject* gObj = ComponentManager::CreateGameObject(id);
				gObj->SetName(name);

				if (obj.HasMember("owner"))
				{
					unsigned int parentID = obj["owner"].GetUint();
					childObjects.emplace_back(gObj, parentID);
				}
			}
		}


		for (auto it = d.MemberBegin(); it != d.MemberEnd();)
		{
			if (it->name != "GameObjects")
			{
				LoadComponent(it->name.GetString(), it->value);
			}

			it++;
		}


		ComponentManager::OnAddedAllComponentsLoadScene();
		ComponentManager::OnLoadScene();

		LoadChildren(childObjects);

		// only if the game has started
		//ComponentManager::AwakeStartComponents();
	}

	void SceneLoader::LoadComponent(const std::string& componentName, const rapidjson::Value& aValue)
	{
		using namespace rapidjson;
		if (!ComponentRegistry::IsRegisteredScene(componentName)) 
			return;

		auto addComponent = ComponentRegistry::GetAddComponent(componentName);

		std::unordered_map<unsigned, Component*> compMap;
		for (const Value& val : aValue.GetArray())
		{
			const unsigned id = val["id"].GetUint();
			const unsigned owner = val["owner"].GetUint();
			Component* component = addComponent(owner, id);

			if (val.HasMember("IsReplicated"))
			{
				bool isReplicated = val["IsReplicated"].GetBool();
				component->IsReplicated = isReplicated;
			}

			compMap.emplace(id, component);
		}

		for (const Value& val : aValue.GetArray())
		{
			const unsigned id = val["id"].GetUint();
			auto& list = Reflection::ReflectionManager::GetList();

			if (compMap.find(id) == compMap.end()) continue;

			Component* pComp = compMap[id];
			auto& varList = list[pComp];

			for (auto* var : varList)
			{
				if (pComp->myInstanceComponentID == id) LoadType(var, val);
			}
		}
	}

	void SceneLoader::LoadType(Reflection::AbstractSerializedVariable* aSerializedVariable, const rapidjson::Value& aValue)
	{
		using namespace rapidjson;
		try
		{
			aSerializedVariable->ResolveTypeInfo();

			if (!aValue.HasMember(aSerializedVariable->GetName()))
				return;

			const Value& val = aValue[aSerializedVariable->GetName()];

			if (aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_String)
			{
				std::string strVal = val.GetString();

				std::string* str = (std::string*)aSerializedVariable->GetData();

				auto vc = strVal.size();
				str->resize(strVal.size());

				memcpy(str->data(), strVal.data(), strVal.size());
			}
			else if (aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_List)
			{
				const unsigned count = val["size"].GetUint();
				aSerializedVariable->Resize(count);

				const std::string strVal = val["data"].GetString();

				std::vector<unsigned char> decoded = Base64::Decode(strVal);
				memcpy(aSerializedVariable->GetData(), decoded.data(), decoded.size());
			}

			else if (
				aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Prefab ||
				aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Texture ||
				aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Material ||
				aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_AudioClip
				)
			{
				std::string id = val.GetString();

				if (aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Prefab)
				{
					Reflection::SerializedVariable<Prefab>* asset = (Reflection::SerializedVariable<Prefab>*)aSerializedVariable;
					*asset = Resources::Get<Prefab>(id);
				}
				if (aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Texture)
				{
					Reflection::SerializedVariable<Texture>* asset = (Reflection::SerializedVariable<Texture>*)aSerializedVariable;
					*asset = Resources::Get<Texture>(id);
				}
				else if (aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_Material)
				{
					Reflection::SerializedVariable<Material>* asset = (Reflection::SerializedVariable<Material>*)aSerializedVariable;
					*asset = Resources::Get<Material>(id);
				}
				else if (aSerializedVariable->GetType() == Reflection::AbstractSerializedVariable::SerializedType_AudioClip)
				{
					Reflection::SerializedVariable<AudioClip>* asset = (Reflection::SerializedVariable<AudioClip>*)aSerializedVariable;
					*asset = Resources::Get<AudioClip>(id);
				}
			}

			else
			{
				std::string strVal = val.GetString();
				std::vector<unsigned char> decoded = Base64::Decode(strVal);
				memcpy(aSerializedVariable->GetData(), decoded.data(), decoded.size());
			}
		}
		catch (const Exception& e)
		{
			LOG_ERROR(e.what());
		}
	}
}