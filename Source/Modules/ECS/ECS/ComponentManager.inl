#include "ComponentManager.h"

#include <cassert>

namespace Eclipse
{
	inline unsigned GetUniqueComponentTypeId()
	{
		static unsigned lastId = 0;
		return lastId++;
	}

	template <typename T>
	inline unsigned ComponentManager::GetComponentID()
	{
		static unsigned componentID = GetUniqueComponentTypeId();
		return componentID;
	}

	template <typename T>
	inline void ComponentManager::GetAllComponentsOfTypePtr(std::vector<int>& aComponents)
	{
		for (int i = 0; i < myComponents.size(); i++)
		{
			auto& component = myComponents[i];

			T* dynamicedComponent = dynamic_cast<T*>(component);
			if (dynamicedComponent)
				aComponents.emplace_back(static_cast<T*>(i));
		}
	}

	template <typename T>
	inline void ComponentManager::GetAllComponentsOfType(unsigned aGOID, std::vector<T*>& aComponents)
	{
		if (myEntityIDToVectorOfComponentIDs.find(aGOID) == myEntityIDToVectorOfComponentIDs.end())
			return;

		auto& entityIDComponents = myEntityIDToVectorOfComponentIDs.at(aGOID);

		for (auto& component : entityIDComponents)
		{
			T* dynamicedComponent = dynamic_cast<T*>(component);
			if (dynamicedComponent)
				aComponents.emplace_back(dynamicedComponent);
		}
	}

	template <typename T>
	inline T* ComponentManager::GetComponent(unsigned aGOID)
	{
		if (myEntityIDToVectorOfComponentIDs.find(aGOID) == myEntityIDToVectorOfComponentIDs.end())
			return nullptr;

		auto& entityIDComponents = myEntityIDToVectorOfComponentIDs.at(aGOID);
		for (auto& component : entityIDComponents)
		{
			T* dynamicCastComponent = dynamic_cast<T*>(component);
			if (dynamicCastComponent)
				return dynamicCastComponent;
		}

		return nullptr;
	}

	// template <typename T>
	// inline T* ComponentManager::GetComponent(unsigned aGOID)
	// {
	//     if (myEntityIDToVectorOfComponentIDs.find(aGOID) == myEntityIDToVectorOfComponentIDs.end())
	//         return nullptr;

	//     unsigned typeIndex = GetComponentID<T>();

	//     auto& entityIDComponents = myEntityIDToVectorOfComponentIDs.at(aGOID);

	//     if (entityIDComponents.find(typeIndex) == entityIDComponents.end())
	//         return nullptr;

	//     int componentIndex = entityIDComponents.at(typeIndex).back();
	//     T* component = static_cast<T*>(myComponents.at(componentIndex));

	//     return component;
	// }

	template <typename T>
	inline T* ComponentManager::AddComponent(unsigned aGOID, bool IsReplicated)
	{
		T* component = AddComponentWithID<T>(aGOID, Component::GetNextComponentID(), IsReplicated);

		component->OnComponentAddedNoCreations();

		component->OnComponentAdded();
		//component->ComponentCreated();

		return component;
	}

	template <typename T>
	inline T* ComponentManager::AddComponentWithID(unsigned aGOID, unsigned aComponentID, bool IsReplicated)
	{
		uint8_t* base = static_cast<uint8_t*>(myComponentData);
		size_t sizeOfNewComponent = sizeof(T);


		uint8_t* ptrToComponent = nullptr;
		for (int i = 0; i < graveyard.size(); i++)
		{
			Graveyard& space = graveyard[i];
			if (space.size == sizeOfNewComponent)
			{
				ptrToComponent = static_cast<uint8_t*>(space.ptr);
				memset(ptrToComponent, 0, space.size);

				std::swap(graveyard[i], graveyard.back());
				graveyard.pop_back();

				break;
			}
		}

		if (!ptrToComponent)
		{
			ptrToComponent = base + myComponentMemoryTracker;

			myComponentMemoryTracker += sizeOfNewComponent;

			assert((myComponentMemoryTracker) <= MAX_COMPONENT_MEMORY_BYTES && "Adding the latest componnet made the component tracker go over max count increase MAX_COMPONENT_MEMORY_BYTES");
		}

		unsigned typeIndex = GetComponentID<T>();

		if (myEntityIdToEntity.find(aGOID) == myEntityIdToEntity.end())
			myEntityIdToEntity.emplace(aGOID, CreateGameObject(aGOID));

#ifdef ECLIPSED_NETWORKING
		//BeforeComponentConstruction();
#endif

		T* component = new(ptrToComponent)T();

		Component* componentPtr = static_cast<Component*>(component);
		componentPtr->componentSize = sizeOfNewComponent;

		component->SetComponentID(aComponentID);

#ifdef ECLIPSED_NETWORKING
		component->IsReplicated = IsReplicated;
		//AfterComponentConstruction();
#endif

		component->gameObject = myEntityIdToEntity.at(aGOID);
		component->myComponentComponentID = typeIndex;

		myComponentsToStartBuffer.emplace_back(component);



		bool SetComponentPtr = false;
		UpdatePriority* componentVectorPtr = nullptr;
		for (auto& componentVec : myComponents)
		{
			if (componentVec.Value == component->GetUpdatePriority())
			{
				componentVectorPtr = &componentVec;
				SetComponentPtr = true;
				break;
			}
		}
		if (!SetComponentPtr)
		{
			componentVectorPtr = &myComponents.emplace_back();
			componentVectorPtr->Value = componentPtr->GetUpdatePriority();
		}
		componentVectorPtr->vector.push_back(component);

		if (SetComponentPtr)
		{
			SortUpdatePrioComponents();
			if (std::is_base_of_v<class BaseRenderComponent, T>)
				SortZIndexComponents(*componentVectorPtr);
		}

		size_t componentIndex = myComponents.size() - 1;

		myEntityIDToVectorOfComponentIDs[aGOID].emplace_back(component);
		//component->myComponentIndex = componentIndex;

#ifdef ECLIPSED_NETWORKING
		if (IsReplicated)
			CreateComponentReplicated(component);
#endif

		if (myComponents.size() <= 1)
			return component;

		return component;
	}
}
