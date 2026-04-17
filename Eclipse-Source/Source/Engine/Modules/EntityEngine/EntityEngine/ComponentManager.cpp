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
        myComponentsToStartBuffer.clear();
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
        for (size_t i = 0; i < myComponentsToStart.size(); ++i)
            if (myComponentsToStart[i]->myIsOwner)
                myComponentsToStart[i]->Awake();
    }

    void ComponentManager::StartComponents()
    {
        for (size_t i = 0; i < myComponentsToStart.size(); ++i)
        {
            if (myComponentsToStart[i]->myIsOwner)
            {
                myComponentsToStart[i]->Start();
                myComponentsToStart[i]->HasStarted = true;
            }
        }
    }

    void ComponentManager::EditorUpdateComponents()
    {
        for (size_t i = 0; i < myComponents.size(); ++i)
            if (myComponents[i]->myIsOwner)
                myComponents[i]->EditorUpdate();
    }

    void ComponentManager::EarlyUpdateComponents()
    {
        for (size_t i = 0; i < myComponents.size(); ++i)
            if (myComponents[i]->myIsOwner && myComponents[i]->HasStarted)
                myComponents[i]->EarlyUpdate();
    }

    void ComponentManager::UpdateComponents()
    {
        for (size_t i = 0; i < myComponents.size(); ++i)
            if (myComponents[i]->myIsOwner && myComponents[i]->HasStarted)
                myComponents[i]->Update();
    }

    void ComponentManager::LateUpdateComponents()
    {
        for (auto& component : myComponents)
            if (component->HasStarted)
                component->OnDrawGizmos();


        for (auto& component : myComponents)
            if (component->myIsOwner && component->HasStarted)
                component->LateUpdate();
    }

    void ComponentManager::RenderComponents()
    {
#ifdef ECLIPSED_EDITOR
        SortComponents();
#endif
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
        // this is a work around so render components does not need to exist should be a separate list
        std::sort(myComponents.begin(), myComponents.end(), [&](Component* aComp0, Component* aComp1)
        {
            const double UpdatePriorityD0 = aComp0->GetUpdatePriority();
            const double UpdatePriorityD1 = aComp1->GetUpdatePriority();

            const double ZindexD0 = aComp0->GetZIndex();
            const double ZindexD1 = aComp1->GetZIndex();

            const double ZindexSmallD0 = ZindexD0 * 0.00000000000001f;
            const double ZindexSmallD1 = ZindexD1 * 0.00000000000001f;

            return UpdatePriorityD0 - ZindexSmallD0 > UpdatePriorityD1 - ZindexSmallD1;
        });

        // myEntityIDToVectorOfComponentIDs.clear();
        //
        // for (int i = 0; i < (int)myComponents.size()/* - 1*/; i++)
        // {
        //     Component* comp = myComponents[i];
        //
        //     auto& mapOfComponentsGO = myEntityIDToVectorOfComponentIDs[comp->gameObject->GetID()];
        //     RegisteredTypeIndex index = comp->myComponentComponentID;
        //
        //     mapOfComponentsGO[index].emplace_back(static_cast<ComponentIndex>(i));
        // }
    }

    Eclipse::Component* ComponentManager::AddComponent(GameObjectID aGOID, Eclipse::Component* (__cdecl*createFunc)(unsigned char* address), size_t size)
    {
        Component* component = AddComponentWithID(aGOID, Component::GetNextComponentID(), createFunc, size);
        component->OnComponentAdded();

        return component;
    }

    Eclipse::Component* ComponentManager::AddComponentWithID(GameObjectID aGOID, unsigned aComponentID, Eclipse::Component* (__cdecl*createFunc)(unsigned char* address), size_t size)
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

        myComponentsToStartBuffer.emplace_back(component);

        myComponents.emplace_back(component);
        size_t componentIndex = myComponents.size() - 1;

        myEntityIDToVectorOfComponentIDs[aGOID].emplace_back(component);
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

        bool DeletedAComponent = false;
        unsigned indexToDelete = 0;
        for (int i = 0; i < entityIDComponents.size(); i++)
        {
            Component* component = entityIDComponents[i];

            if (component->myInstanceComponentID != aComponentID)
                continue;

            component->OnDestroy();
            component->~Component();

            component->IsDeleted = true;

            DeleteReplicatedComponent(aComponentID);

            indexToDelete = i;

            DeletedAComponent = true;

            break;
        }

        if (!DeletedAComponent)
            return;

        entityIDComponents[indexToDelete] = entityIDComponents.back();
        entityIDComponents.pop_back();

        for (auto& component : myComponents)
        {
            if (component->IsDeleted)
            {
                component = myComponents.back();
                break;
            }
        }

        myComponents.pop_back();

        SortComponents();
    }

    GameObject* ComponentManager::FindObjectByName(const char* aName)
    {
        for (auto& [id, gameobject] : myEntityIdToEntity)
            if (gameobject->GetName() == aName)
                return gameobject;

        return nullptr;
    }

    const std::vector<Component*>& ComponentManager::GetAllComponents()
    {
        return myComponents;
    }

    std::vector<Component*> ComponentManager::GetComponents(GameObjectID aGOID)
    {
        if (myEntityIDToVectorOfComponentIDs.find(aGOID) == myEntityIDToVectorOfComponentIDs.end())
            return {};

        return myEntityIDToVectorOfComponentIDs.at(aGOID);
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

        unsigned componentsDeleted = 0;
        for (int goID : gameobjectsToRemove)
        {
            for (auto& component : myEntityIDToVectorOfComponentIDs.at(goID))
            {
                DeleteReplicatedComponent(component->myInstanceComponentID);

                component->IsDeleted = true;

                component->OnDestroy();
                component->~Component();
                component = nullptr;

                componentsDeleted++;
            }
        }

        for (int i = 0; i < myComponents.size(); i++)
        {
            auto& component = myComponents[i];
            if (!component->IsDeleted)
                continue;

            Component* backComponent = myComponents.back();
            if (!backComponent->IsDeleted)
                std::swap(myComponents[i], myComponents.back());

            myComponents.pop_back();
            i--;
        }

        {
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
        }

        {
            int componentToStartSize = 0;
            for (int i = myComponentsToStartBuffer.size() - 1; i >= 0; i--)
            {
                auto& component = myComponentsToStartBuffer[i];
                if (component->IsDeleted)
                {
                    std::swap(component, myComponentsToStartBuffer[myComponentsToStartBuffer.size() - componentToStartSize - 1]);
                    componentToStartSize++;
                }
            }
            myComponentsToStartBuffer.resize(myComponentsToStartBuffer.size() - componentToStartSize);
        }

        SortComponents();

        for (int goID : gameobjectsToRemove)
        {
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
        auto& allComponents = myEntityIDToVectorOfComponentIDs[aGOID];

        for (auto& component : allComponents)
            component->OnCollisionEnter();
    }

    void ComponentManager::EndCollisions(GameObjectID aGOID)
    {
        auto& allComponents = myEntityIDToVectorOfComponentIDs[aGOID];

        for (auto& component : allComponents)
            component->OnCollisionExit();
    }
}
