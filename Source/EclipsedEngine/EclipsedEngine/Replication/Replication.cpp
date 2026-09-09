#ifdef ECLIPSED_NETWORKING

#include "Replication.h"

#include "Networking/Shared/Message.h"

#include "Networking/Server/SteamP2PNetworkingServer.h"
#include "Networking/Client/SteamP2PNetworkingClient.h"

#include "Renderer/RenderCommands/CommandList.h"
#include "ECS/ComponentManager.h"
#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"
#include "EclipsedEngine/Replication/ReplicationManager.h"
#include "EclipsedEngine/Replication/ReplicatedVariable.h"

#include "EclipsedEngine/ECS/ObjectManager.h"

#include "Assets/AssetManager.h"

#include "Assets/Assets/PrefabAsset.h"

namespace Eclipse::Replication
{
	void ReplicationHelper::ClientHelp::RecieveAddComponentMessage(const NetMessage& message)
	{
		// if (!ComponentManager::Get().HasGameObject(message.MetaData.GameObjectID))
		// {
		//     GameObject* gameobject = ComponentManager::Get().CreateGameObject(message.MetaData.GameObjectID);
		//     gameobject->SetIsOwner(true);
		// }

		int ComponentID = 0;
		memcpy(&ComponentID, message.data, sizeof(int));

		int ComponentNameCharCount = 0;
		memcpy(&ComponentNameCharCount, message.data + sizeof(int), sizeof(int));

		char NameBuffer[512];

		memcpy(NameBuffer, message.data + sizeof(int) + sizeof(int), ComponentNameCharCount);
		memset(NameBuffer + ComponentNameCharCount, '\0', 1);

		const char* name = NameBuffer;

		for (auto& component : ComponentManager::GetComponents(message.MetaData.GameObjectID))
		{
			const char* CurrentComponentName = component->GetComponentName();
			if (!memcmp(name, CurrentComponentName, strlen(name)))
				return;
		}

		std::string StringName = name;

		Component* newCompoennt = ComponentRegistry::GetAddComponentWithIdByTypeName(StringName)(message.MetaData.GameObjectID, ComponentID);
		newCompoennt->SetIsOwner(false);
		newCompoennt->IsReplicated = true;

		//ComponentsToStartOnDemand.emplace_back(newCompoennt);

		ComponentsToStartOnDemand.emplace_back(newCompoennt);
		if (ComponentsToStartOnDemand.size() >= myComponentsToRecieved)
		{
			StartReplicatedComponents();
			myComponentsToRecieved = 9999;
		}
	}

	void ReplicationHelper::ClientHelp::RecieveAmountOfComponents(const NetMessage& message)
	{
		memcpy(&myComponentsToRecieved, message.data, sizeof(size_t));
	}

	void ReplicationHelper::ClientHelp::StartReplicatedComponents()
	{
		Graphics::CommandListManager::GetHappenAtBeginCommandList().Enqueue([]()
			{
				std::sort(ComponentsToStartOnDemand.begin(), ComponentsToStartOnDemand.end(), [&](Component* aComp0, Component* aComp1)
					{
						return aComp0->GetUpdatePriority() > aComp1->GetUpdatePriority();
					});
				for (Component* component : ComponentsToStartOnDemand)
				{
					component->OnComponentAddedNoCreations();
					component->OnComponentAdded();

					component->Awake();
					component->Start();
				}
				ComponentsToStartOnDemand.clear();
			});
	}

	void ReplicationHelper::ClientHelp::RecieveCreateObjectMessage(const NetMessage& message)
	{
		if (ComponentManager::HasGameObject(message.MetaData.GameObjectID))
			return;

		GameObject* gameobject = ComponentManager::CreateGameObject(message.MetaData.GameObjectID);
		gameobject->SetIsOwner(false);
	}

	void ReplicationHelper::ClientHelp::RecieveDeleteObjectMessage(const NetMessage& message)
	{
		if (!ComponentManager::HasGameObject(message.MetaData.GameObjectID))
			return;

		ComponentManager::Destroy(message.MetaData.GameObjectID);
	}

	void ReplicationHelper::ClientHelp::RecieveRequestVariablesMessage(const NetMessage& message)
	{
		const auto& variableManager = *Replication::ReplicationManager::CurrentReplicatedVariabpePtr;
		for (auto& Component : variableManager)
		{
			for (int i = 0; i < Component.second.size(); i++)
			{
				auto& Variable = Component.second[i];
				if (Variable->OnComponent->IsReplicated)
				{
					Variable->ReplicateThis(i, true);
				}
			}
		}
	}

	void ReplicationHelper::ClientHelp::RecieveVariableMessage(const NetMessage& message)
	{
		unsigned componentID = 0;
		unsigned iterationID = 0;
		int dataAmount = 0;

		size_t offset = 0;

		memcpy(&componentID, message.data + offset, sizeof(componentID));
		offset += sizeof(componentID);

		memcpy(&iterationID, message.data + offset, sizeof(iterationID));
		offset += sizeof(iterationID);

		memcpy(&dataAmount, message.data + offset, sizeof(dataAmount));
		offset += sizeof(dataAmount);

		auto variableIt = Replication::ReplicationManager::CurrentReplicatedVariabpePtr->find(componentID);
		if (variableIt == Replication::ReplicationManager::CurrentReplicatedVariabpePtr->end())
			return;

		auto Variable = reinterpret_cast<Replication::ReplicatedVariable<Component>*>(variableIt->second[iterationID]);


		//if (Variable->IsAsset)
		//{
		//	Assets::GUID AssetID;
		//	memcpy(&AssetID, message.data + offset, sizeof(Assets::GUID));
		//	RefreshAsset(Variable->myReflectVariable, AssetID);
		//}
		//else
		//{
		void* variableData = Variable->Data;
		memcpy(variableData, message.data + offset, dataAmount);
		//}

		Component* component = Variable->OnComponent;
		const auto& ReppedFunction = Variable->OnRepFunction;
		(component->*ReppedFunction)();
	}

	void InstatiateNetworkSentPrefab(Assets::Prefab& aPrefab, int gameobjectID, std::vector<unsigned> aComponents, bool Replicated = false)
	{
		GameObject* gameobject = InternalSpawnObjectClass::CreateObjectFromJsonStringSpecifiedIds(aPrefab.dataPtr->data, gameobjectID, aComponents, true);
		gameobject->prefabAssetIDStr = aPrefab.GetAssetID().ToString();

		gameobject->IsPrefab = true;

		aPrefab.dataPtr->gameobject = gameobject;
	}

	void ReplicationHelper::ClientHelp::RecieveInstantiatePrefabMessage(const NetMessage& message)
	{
		if (ComponentManager::HasGameObject(message.MetaData.GameObjectID))
			return;

		Graphics::CommandListManager::GetHappenAtBeginCommandList().Enqueue([message]()
			{
				char* prefabID = (char*)malloc(32);
				memcpy(prefabID, message.data, 32);
				memset(prefabID + 32, 0, 1);
				int offset = 32;

				unsigned componentCount;
				memcpy(&componentCount, message.data + offset, sizeof(unsigned));
				offset += sizeof(unsigned);

				std::vector<unsigned> componentsIDs;
				componentsIDs.resize(componentCount);
				memcpy(componentsIDs.data(), message.data + offset, sizeof(unsigned) * componentCount);

				Eclipse::Assets::GUID guid;
				guid.FromString(prefabID);

				Eclipse::Assets::Prefab prefab = Assets::AssetManager::Load<Eclipse::Assets::Prefab>(guid);

				InstatiateNetworkSentPrefab(prefab, message.MetaData.GameObjectID, componentsIDs);
			});
	}

	void ReplicationHelper::ClientHelp::HandleRecieve(const NetMessage& message)
	{
		switch (message.MetaData.Type)
		{
		case MessageType::Msg_RequestVariables:
		{
			ReplicationHelper::ClientHelp::RecieveRequestVariablesMessage(message);
		}
		break;
		case MessageType::Msg_Variable:
		{
			ReplicationHelper::ClientHelp::RecieveVariableMessage(message);
		}
		break;
		case MessageType::Msg_InstantiatePrefab:
		{
			ReplicationHelper::ClientHelp::RecieveInstantiatePrefabMessage(message);
		}
		break;
		case MessageType::Msg_CreateObject:
		{
			ReplicationHelper::ClientHelp::RecieveCreateObjectMessage(message);
		}
		break;
		case MessageType::Msg_SendMultipleComponents:
		{
			ReplicationHelper::ClientHelp::RecieveAmountOfComponents(message);
		}
		break;
		case MessageType::Msg_DeleteObject:
		{
			ReplicationHelper::ClientHelp::RecieveDeleteObjectMessage(message);
		}
		break;
		case MessageType::Msg_AddComponent:
		{
			ReplicationHelper::ClientHelp::RecieveAddComponentMessage(message);
		}
		break;
		}
	}


	void ReplicationHelper::ClientHelp::RefreshAsset(Reflection::AbstractSerializedVariable* aVariable, Assets::GUID aAssetID)
	{
		aVariable->ResolveTypeInfo();

		switch (aVariable->GetType())
		{
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_Material:
		{
			Assets::Material& material = *(static_cast<Assets::Material*>(aVariable->GetData()));
			material = Assets::AssetManager::Load<Assets::Material>(aAssetID);
		}
		break;
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_AudioClip:
		{
			Assets::AudioClip& assset = *(static_cast<Assets::AudioClip*>(aVariable->GetData()));
			assset = Assets::AssetManager::Load<Assets::AudioClip>(aAssetID);
		}
		break;
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_Texture:
		{
			Assets::Texture& assset = *(static_cast<Assets::Texture*>(aVariable->GetData()));
			assset = Assets::AssetManager::Load<Assets::Texture>(aAssetID);
		}
		break;
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_Prefab:
		{
			Assets::Prefab& assset = *(static_cast<Assets::Prefab*>(aVariable->GetData()));
			assset = Assets::AssetManager::Load<Assets::Prefab>(aAssetID);
		}
		break;
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_Font:
		{
			Assets::Font& assset = *(static_cast<Assets::Font*>(aVariable->GetData()));
			assset = Assets::AssetManager::Load<Assets::Font>(aAssetID);
		}
		break;
		}
	}

	void ReplicationHelper::ServerHelp::RequestVariablesFromClient()
	{
		SteamP2PNetworkingServer& server = Eclipse::MainSingleton::GetInstance<SteamP2PNetworkingServer>();
		NetMessage msg = NetMessage::BuildGameObjectMessage(0, MessageType::Msg_RequestVariables, nullptr, 0, true);
		server.Send(msg);
	}

	void ReplicationHelper::ServerHelp::SendComponentsScene()
	{
		std::unordered_set<unsigned> ReplicatedGameObjects;

		const auto& variableManager = *Replication::ReplicationManager::CurrentReplicatedVariabpePtr;
		for (auto& Variable : variableManager)
		{
			Component*& currentComponent = Variable.second[0]->OnComponent;
			if (!currentComponent->IsReplicated)
				continue;

			unsigned gameobjectID = currentComponent->gameObject->GetID();
			ReplicatedGameObjects.emplace(gameobjectID);
		}

		int ComponentCount = 0;


		std::vector<Component*> componentsToReplicate;

		const ComponentManager::RenderLayers& allComponents = ComponentManager::GetAllComponents();
		for (const auto& componentList : allComponents)
		{
			for (const auto& component : componentList.vector)
			{
				if (component->IsReplicated && component->IsOwner() && !component->gameObject->IsPrefab)
				{
					ComponentCount++;
					componentsToReplicate.emplace_back(component);
				}
			}
		}

		SteamP2PNetworkingServer& server = Eclipse::MainSingleton::GetInstance<SteamP2PNetworkingServer>();

		if (componentsToReplicate.size())
		{
			NetMessage msg = NetMessage::BuildGameObjectMessage(0, MessageType::Msg_SendMultipleComponents, &ComponentCount, sizeof(unsigned), true);
			server.Send(msg);
		}

		for (const auto& gameobject : ReplicatedGameObjects)
		{
			GameObject* object = ComponentManager::GetGameObject(gameobject);
			if (object->IsPrefab)
			{
				Assets::GUID guid;
				guid.FromString(object->prefabAssetIDStr);

				Assets::Prefab prefab = Assets::AssetManager::Load<Assets::Prefab>(guid);
				ReplicationManager::SendPrefabObject(object, prefab);
			}
		}

		if (componentsToReplicate.size())
		{
			for (const auto& component : componentsToReplicate)
			{
				NetMessage message;
				ReplicationManager::CreateComponentMessage(component, message, true);
				server.Send(message);
			}
		}
	}

	void ReplicationHelper::ServerHelp::HandleRequestedScene()
	{
		SendComponentsScene();
	}

	void ReplicationHelper::ServerHelp::HandleRecieve(const NetMessage& aMessage)
	{
		switch (aMessage.MetaData.Type)
		{
		case MessageType::Msg_RequestSceneInfo:
			HandleRequestedScene();
			break;

		default:
			ClientHelp::HandleRecieve(aMessage);
			break;
		}
	}
}

#endif