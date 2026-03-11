#include "ComponentManager.h"

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

        //unsigned typeIndex = GetComponentID<T>();

        auto& entityIDComponents = myEntityIDToVectorOfComponentIDs.at(aGOID);

        for (auto& components : entityIDComponents)
        {
            T* dynamicedComponent = dynamic_cast<T*>(myComponents.at(components.second.back()));
            if (dynamicedComponent)
            {
                for (auto& curComponent : components.second)
                {
                    T* component = static_cast<T*>(myComponents.at(curComponent));
                    aComponents.emplace_back(component);
                }
            }
        }
    }

    template <typename T>
    inline T* ComponentManager::GetComponent(unsigned aGOID)
    {
        if (myEntityIDToVectorOfComponentIDs.find(aGOID) == myEntityIDToVectorOfComponentIDs.end())
            return nullptr;

        auto& entityIDComponents = myEntityIDToVectorOfComponentIDs.at(aGOID);
        for (auto& components : entityIDComponents)
        {
            for (auto& component : components.second)
            {
                T* dynamicedComponent = dynamic_cast<T*>(myComponents.at(component));
                if (dynamicedComponent)
                    return dynamicedComponent;
            }
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

    template<typename T>
    inline T* ComponentManager::AddComponentWithID(unsigned aGOID, unsigned aComponentID, bool IsReplicated)
    {
        uint8_t* base = static_cast<uint8_t*>(myComponentData);
        uint8_t* ptrToComponent = base + myComponentMemoryTracker;
        size_t sizeOfNewComponent = sizeof(T);
        myComponentMemoryTracker += sizeOfNewComponent;

        assert((myComponentMemoryTracker) <= MAX_COMPONENT_MEMORY_BYTES && "Adding the latest componnet made the component tracker go over max count increase MAX_COMPONENT_MEMORY_BYTES");

        unsigned typeIndex = GetComponentID<T>();

        if (myEntityIdToEntity.find(aGOID) == myEntityIdToEntity.end())
            myEntityIdToEntity.emplace(aGOID, CreateGameObject(aGOID));

        BeforeComponentConstruction();

        T* component = new(ptrToComponent)T();
        component->SetComponentID(aComponentID);
        component->IsReplicated = IsReplicated;

        AfterComponentConstruction();


        component->gameObject = myEntityIdToEntity.at(aGOID);
        component->myComponentComponentID = typeIndex;

        myComponentsToStart.emplace_back(component);

        myComponents.emplace_back(component);
        size_t componentIndex = myComponents.size() - 1;

        myEntityIDToVectorOfComponentIDs[aGOID][typeIndex].emplace_back(static_cast<unsigned>(componentIndex));
        myComponents.back()->myComponentIndex = componentIndex;
        

        if (IsReplicated)
            CreateComponentReplicated(component);

        if (myComponents.size() <= 1)
            return component;

        SortComponents();

        return component;
    }
}