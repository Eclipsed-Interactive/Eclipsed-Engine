#include "ComponentManager.h"

#include "CoreEngine/IDGenerator.h"

namespace Eclipse
{
	void ComponentManager::Init()
	{
		myComponentData = reinterpret_cast<uint8_t*>(malloc(MAX_COMPONENT_MEMORY_BYTES));
	}

	void ComponentManager::OnLoadScene()
	{
		SortComponents();

		for (auto& component : myComponents)
			component->OnSceneLoaded();
	}

	void ComponentManager::OnAddedAllComponentsLoadScene()
	{
		SortComponents();

		for (auto& component : myComponents)
		{
			component->OnComponentAddedNoCreations();

			component->OnComponentAdded();
		}
	}

	void ComponentManager::AwakeStartComponents()
	{
		if (myComponentsToStart.empty() && myComponentsToStartBuffer.empty())
			return;

		std::sort(myComponentsToStart.begin(), myComponentsToStart.end(), [&](Component* aComp0, Component* aComp1)
			{
				bool hasPriority = aComp0->GetUpdatePriority() > aComp1->GetUpdatePriority();
				return hasPriority;
			});

		AwakeComponents();
		StartComponents();

		myComponentsToStart.clear();
		myComponentsToStart = myComponentsToStartBuffer;
		myComponentsToStartBuffer.clear();
	}

	void ComponentManager::Clear()
	{
		for (auto& component : myComponents)
		{
			component->OnDestroy();
			component->~Component();
		}

		myComponents.clear();
		free(myComponentData);

		myComponentMemoryTracker = 0;
		myComponentsToStart.clear();

		for (auto& [id, obj] : myEntityIdToEntity)
		{
			delete obj;
		}

		myEntityIdToEntity.clear();
		myEntityIDToVectorOfComponentIDs.clear();

		Init();
	}

	void ComponentManager::AwakeComponents()
	{
		for (int i = 0; i < myComponentsToStart.size(); i++)
		{
			auto& component = myComponentsToStart[i];
			if (component->myIsOwner)
				component->Awake();
		}
	}
	void ComponentManager::StartComponents()
	{
		for (int i = 0; i < myComponentsToStart.size(); i++)
		{
			auto& component = myComponentsToStart[i];
			if (component->myIsOwner)
				component->Start();
		}
	}

	void ComponentManager::EditorUpdateComponents()
	{
		for (auto& component : myComponents)
			component->EditorUpdate();
	}
	void ComponentManager::EarlyUpdateComponents()
	{
		for (auto& component : myComponents)
			if (component->myIsOwner)
				component->EarlyUpdate();
	}
	void ComponentManager::UpdateComponents()
	{
		for (auto& component : myComponents)
			if (component->myIsOwner)
				component->Update();
	}
	void ComponentManager::LateUpdateComponents()
	{
		for (auto& component : myComponents)
			component->OnDrawGizmos();

		for (auto& component : myComponents)
			if (component->myIsOwner)
				component->LateUpdate();
	}
	void ComponentManager::RenderComponents()
	{
		for (auto& component : myComponents)
			component->Render();
	}
	void ComponentManager::AfterRenderUpdateComponents()
	{
		for (auto& component : myComponents)
			component->AfterRenderUpdate();
	}
	void ComponentManager::SortComponents()
	{
		std::sort(myComponents.begin(), myComponents.end(), [&](Component* aComp0, Component* aComp1)
			{
				return aComp0->GetUpdatePriority() > aComp1->GetUpdatePriority();
			});

		myEntityIDToVectorOfComponentIDs.clear();

		for (int i = 0; i < (int)myComponents.size()/* - 1*/; i++)
		{
			Component* comp = myComponents[i];

			auto& mapOfComponentsGO = myEntityIDToVectorOfComponentIDs[comp->gameObject->GetID()];
			RegisteredTypeIndex index = comp->myComponentComponentID;

			mapOfComponentsGO[index].emplace_back(static_cast<ComponentIndex>(i));
		}
	}

	Eclipse::Component* ComponentManager::AddComponent(GameObjectID aGOID, Eclipse::Component* (__cdecl* createFunc)(unsigned char* address), size_t size)
	{
		Component* component = AddComponentWithID(aGOID, Component::GetNextComponentID(), createFunc, size);
		component->OnComponentAdded();

		return component;
	}

	Eclipse::Component* ComponentManager::AddComponentWithID(GameObjectID aGOID, unsigned aComponentID, Eclipse::Component* (__cdecl* createFunc)(unsigned char* address), size_t size)
	{
		uint8_t* base = static_cast<uint8_t*>(myComponentData);
		uint8_t* ptrToComponent = base + myComponentMemoryTracker;
		myComponentMemoryTracker += size;

		assert((myComponentMemoryTracker) <= MAX_COMPONENT_MEMORY_BYTES && "Adding the latest componnet made the component tracker go over max count increase MAX_COMPONENT_MEMORY_BYTES");


		unsigned typeIndex = Utilities::IDGenerator::GetID();

		if (myEntityIdToEntity.find(aGOID) == myEntityIdToEntity.end())
			myEntityIdToEntity[aGOID] = CreateGameObject(aGOID);


		BeforeComponentConstruction();

		Eclipse::Component* component = createFunc(ptrToComponent);
		component->SetComponentID(aComponentID);

		AfterComponentConstruction();

		component->gameObject = myEntityIdToEntity.at(aGOID);
		component->myComponentComponentID = typeIndex;

		myComponentsToStart.emplace_back(component);

		myComponents.emplace_back(component);
		size_t componentIndex = myComponents.size() - 1;

		myEntityIDToVectorOfComponentIDs[aGOID][typeIndex].emplace_back(static_cast<unsigned>(componentIndex));
		myComponents.back()->myComponentIndex = componentIndex;

		//CreateComponentReplicated(component);

		if (myComponents.size() <= 1)
			return component;

		SortComponents();

		return component;
	}

	inline void ComponentManager::DeleteComponent(unsigned aGOID, unsigned aUniqueComponentID, unsigned aComponentID)
	{
		if (myEntityIDToVectorOfComponentIDs.find(aGOID) == myEntityIDToVectorOfComponentIDs.end())
			return;

		auto& entityIDComponents = myEntityIDToVectorOfComponentIDs.at(aGOID);

		if (entityIDComponents.find(aUniqueComponentID) == entityIDComponents.end())
			return;

		std::vector<unsigned>& componentIndex = entityIDComponents.at(aUniqueComponentID);

		unsigned indexToDelete = -1;
		unsigned compIndex = 0;
		for (int i = 0; i < componentIndex.size(); i++)
		{
			compIndex = componentIndex[i];
			Component* component = myComponents[compIndex];

			if (component->myInstanceComponentID != aComponentID)
				continue;

			component->OnDestroy();
			component->~Component();

			DeleteReplicatedComponent(aComponentID);

			indexToDelete = i;

			break;
		}

		if (indexToDelete == -1)
			return;

		componentIndex[indexToDelete] = componentIndex.back();
		componentIndex.pop_back();

		if (!componentIndex.size())
			entityIDComponents.erase(aUniqueComponentID);

		myComponents[compIndex] = myComponents.back();
		myComponents.pop_back();

		SortComponents();
	}

	const std::vector<Component*>& ComponentManager::GetAllComponents()
	{
		return myComponents;
	}

	std::vector<Component*> ComponentManager::GetComponents(GameObjectID aGOID)
	{
		if (myEntityIDToVectorOfComponentIDs.find(aGOID) == myEntityIDToVectorOfComponentIDs.end())
			return {};

		std::vector<Component*> components;

		for (auto& goComponents : myEntityIDToVectorOfComponentIDs.at(aGOID))
		{
			for (auto& component : goComponents.second)
			{
				components.emplace_back(myComponents[component]);
			}
		}

		return components;
	}

	bool ComponentManager::HasGameObject(GameObjectID aGOID)
	{
		return myEntityIdToEntity.find(aGOID) != myEntityIdToEntity.end();
	}

	GameObject* ComponentManager::GetGameObject(unsigned aGOID)
	{
		if (myEntityIdToEntity.find(aGOID) == myEntityIdToEntity.end())
			return nullptr;

		return myEntityIdToEntity.at(aGOID);
	}

	void ComponentManager::CommitDestroy()
	{
		if (gameobjectsToRemove.empty())
			return;

		for (int goID : gameobjectsToRemove)
		{
			std::vector<int> componentsToRemove;

			for (auto& componentsAtGO : myEntityIDToVectorOfComponentIDs.at(goID))
			{
				for (auto& componentAtGO : componentsAtGO.second)
				{
					Component*& component = myComponents[componentAtGO];

					DeleteReplicatedComponent(component->myInstanceComponentID);

					component->IsDeleted = true;

					component->OnDestroy();
					component->~Component();

					component = nullptr;

					//memset(component, 0, sizeof(component));

					componentsToRemove.emplace_back(componentAtGO);
				}
			}

			int sizeOfComponents = myComponents.size();
			int sizeOfComponentsRemove = componentsToRemove.size();
			int sizeAfterRemoval = (sizeOfComponents - sizeOfComponentsRemove);

			std::sort(componentsToRemove.begin(), componentsToRemove.end());

			std::vector<bool> used(sizeOfComponents, false);

			for (int i = sizeOfComponentsRemove - 1; i >= 0; i--)
			{
				int componentIndex = componentsToRemove[i];
				std::swap(myComponents[componentIndex], myComponents.back());
				myComponents.pop_back();
			}

			int componentToStartSize = 0;
			for (int i = myComponentsToStart.size() - 1; i >= 0; i--)
			{
				auto& component = myComponentsToStart[i];
				if (component->IsDeleted)
				{
					std::swap(component, myComponentsToStart[myComponentsToStart.size() - componentToStartSize - 1]);
					componentToStartSize++;
				}
			}

			myComponentsToStart.resize(myComponentsToStart.size() - componentToStartSize);

			SortComponents();

			delete myEntityIdToEntity.at(goID);
			myEntityIdToEntity.erase(goID);
		}

		gameobjectsToRemove.clear();
	}

	void ComponentManager::Destroy(GameObjectID aGOID)
	{
		DestroyGameObjectReplicated(aGOID);

		gameobjectsToRemove.emplace_back(aGOID);
	}

	GameObject* ComponentManager::CreateGameObject(GameObjectID aId)
	{
		GameObject* obj = new GameObject(aId);
		myEntityIdToEntity[aId] = obj;

		return obj;
	}

	GameObject* ComponentManager::CreateGameObject()
	{
		unsigned randomID = GetNewGOID();
		GameObject* obj = new GameObject(randomID);
		myEntityIdToEntity[randomID] = obj;

		return obj;
	}


	void ComponentManager::BeginCollisions(GameObjectID aGOID)
	{
		auto& components = myEntityIDToVectorOfComponentIDs[aGOID];

		for (auto& component : components)
			for (auto& [_, componentsIndex] : components)
				for (auto& componentIndex : componentsIndex)
					myComponents[componentIndex]->OnCollisionEnter();
	}
	void ComponentManager::EndCollisions(GameObjectID aGOID)
	{
		auto& components = myEntityIDToVectorOfComponentIDs[aGOID];

		for (auto& component : components)
			for (auto& [_, componentsIndex] : components)
				for (auto& componentIndex : componentsIndex)
					myComponents[componentIndex]->OnCollisionExit();
	}
}