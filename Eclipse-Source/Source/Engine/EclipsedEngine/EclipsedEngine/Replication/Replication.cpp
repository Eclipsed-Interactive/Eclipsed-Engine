#include "Replication.h"

#include "NetworkEngine/Shared/Message.h"

#include "NetworkEngine/Server/SteamP2PNetworkingServer.h"
#include "NetworkEngine/Client/SteamP2PNetworkingClient.h"

#include "GraphicsEngine/RenderCommands/CommandList.h"
#include "EntityEngine/ComponentManager.h"
#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"
#include "EclipsedEngine/Replication/ReplicationManager.h"
#include "EclipsedEngine/Replication/ReplicatedVariable.h"

#include "EclipsedEngine/ECS/SpawnObject.h"

#include "AssetEngine/Resources.h"

namespace Eclipse::Replication
{
	void ReplicationHelper::ClientHelp::RecieveAddComponentMessage(const NetMessage& message)
	{
		// if (!ComponentManager::HasGameObject(message.MetaData.GameObjectID))
		// {
		//     GameObject* gameobject = ComponentManager::CreateGameObject(message.MetaData.GameObjectID);
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

		Component* newCompoennt = ComponentRegistry::GetAddComponent(StringName)(message.MetaData.GameObjectID, ComponentID);
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
		CommandListManager::GetHappenAtBeginCommandList().Enqueue([]() {
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
		const auto& variableManager = Replication::ReplicationManager::RealReplicatedVariableList;
		for (auto& Component : variableManager)
		{
			for (int i = 0; i < Component.second.size(); i++)
			{
				auto& Variable = Component.second[i];
				if (Variable->ConnectedComponent->IsReplicated)
				{
					Variable->ReplicateThis(i, true);
				}
			}
		}
	}

	void ReplicationHelper::ClientHelp::RecieveVariableMessage(const NetMessage& message)
	{
		unsigned replicationVarIndex = 0;
		unsigned componentID = 0;
		int dataAmount = 0;

		size_t offset = 0;

		memcpy(&componentID, message.data + offset, sizeof(componentID));
		offset += sizeof(componentID);

		memcpy(&replicationVarIndex, message.data + offset, sizeof(replicationVarIndex));
		offset += sizeof(replicationVarIndex);

		memcpy(&dataAmount, message.data + offset, sizeof(dataAmount));
		offset += sizeof(dataAmount);

		auto variableIt = Replication::ReplicationManager::RealReplicatedVariableList.find(componentID);
		if (variableIt == Replication::ReplicationManager::RealReplicatedVariableList.end())
			return;

		Replication::ReplicatedVariable<Component>* Variable = reinterpret_cast<Replication::ReplicatedVariable<Component>*>(variableIt->second[replicationVarIndex]);

		Component* component = Variable->ConnectedComponent;
		const auto& ReppedFunction = Variable->OnRepFunction;

		if (Variable->IsAsset)
		{
			std::string AssetID = "";
			memcpy(&AssetID, message.data + offset, 32);
			RefreshAsset(Variable->myReflectVariable, AssetID);
		}
		else
		{
			void* variableData = Variable->myReflectVariable->GetData();
			memcpy(variableData, message.data + offset, dataAmount);
		}

		(component->*ReppedFunction)();
	}

	void InstatiateNetworkSentPrefab(Prefab& aPrefab, int gameobjectID, std::vector<unsigned> aComponents, bool Replicated = false)
	{
		GameObject* gameobject = InternalSpawnObjectClass::CreateObjectFromJsonStringSpecifiedIds(aPrefab.GetData()->data, gameobjectID, aComponents);
		gameobject->prefabAssetID = aPrefab.GetAssetID();

		gameobject->IsPrefab = true;

		aPrefab.GetData()->gameobject = gameobject;
	}

	void ReplicationHelper::ClientHelp::RecieveInstantiatePrefabMessage(const NetMessage& message)
	{
		if (ComponentManager::HasGameObject(message.MetaData.GameObjectID))
			return;

		CommandListManager::GetHappenAtBeginCommandList().Enqueue([message]() {

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

			Eclipse::Prefab prefab = Eclipse::Resources::Get<Eclipse::Prefab>(prefabID);

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


	void ReplicationHelper::ClientHelp::RefreshAsset(Reflection::AbstractSerializedVariable* aVariable, std::string aAssetID)
	{
		aVariable->ResolveTypeInfo();

		switch (aVariable->GetType())
		{
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_Material:
		{
			Eclipse::Material& material = *(static_cast<Eclipse::Material*>(aVariable->GetData()));
			material = Eclipse::Resources::Get<Eclipse::Material>(aAssetID);
		}
		break;
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_AudioClip:
		{
			Eclipse::AudioClip& assset = *(static_cast<Eclipse::AudioClip*>(aVariable->GetData()));
			assset = Eclipse::Resources::Get<Eclipse::AudioClip>(aAssetID);
		}
		break;
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_Texture:
		{
			Eclipse::Texture& assset = *(static_cast<Eclipse::Texture*>(aVariable->GetData()));
			assset = Eclipse::Resources::Get<Eclipse::Texture>(aAssetID);
		}
		break;
		case Eclipse::Reflection::AbstractSerializedVariable::SerializedType_Prefab:
		{
			Eclipse::Prefab& assset = *(static_cast<Eclipse::Prefab*>(aVariable->GetData()));
			assset = Eclipse::Resources::Get<Eclipse::Prefab>(aAssetID);
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

		const auto& variableManager = Replication::ReplicationManager::RealReplicatedVariableList;
		for (auto& Variable : variableManager)
		{
			unsigned gameobjectID = Variable.second[0]->ConnectedComponent->gameObject->GetID();
			ReplicatedGameObjects.emplace(gameobjectID);
		}

		int ComponentCount = 0;


		std::vector<Component*> componentsToReplicate;

		const std::vector<Component*>& allComponents = ComponentManager::GetAllComponents();
		for (const auto& component : allComponents)
		{
			if (component->IsReplicated && component->IsOwner() && !component->gameObject->IsPrefab)
			{
				ComponentCount++;
				componentsToReplicate.emplace_back(component);
			}
		}

		SteamP2PNetworkingServer& server = Eclipse::MainSingleton::GetInstance<SteamP2PNetworkingServer>();

		NetMessage msg = NetMessage::BuildGameObjectMessage(0, MessageType::Msg_SendMultipleComponents, &ComponentCount, sizeof(unsigned), true);
		server.Send(msg);

		for (const auto& gameobject : ReplicatedGameObjects)
		{
			GameObject* object = ComponentManager::GetGameObject(gameobject);
			if (object->IsPrefab)
			{
				Prefab prefab = Eclipse::Resources::Get<Prefab>(object->prefabAssetID);
				Replication::ReplicationManager::SendPrefabObject(object, prefab);

				continue;
			}
			
			for (const auto& component : componentsToReplicate)
			{
				NetMessage message;
				Replication::ReplicationManager::CreateComponentMessage(component, message, true);
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
			break;
		}
	}
}