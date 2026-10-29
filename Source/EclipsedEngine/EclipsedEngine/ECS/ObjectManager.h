#pragma once

#include <vector>

#include "rapidjson/document.h"

#include "EclipsedEngine/Replication/ReplicationManager.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "ECS/GameObject.h"

#include "Assets/AssetManager.h"

#include "EclipsedEngine.Core.hpp"
#include <Assets/Assets/PrefabAsset.h>


namespace Eclipse
{
    class ECL_API InternalSpawnObjectClass
    {
    public:
        static GameObject* CreateObjectFromJsonString(const char* aData);
        static GameObject* CreateObjectFromJsonStringSpecifiedIds(const char* aData,
            int aGameobjectID, const std::vector<unsigned>& aComponentsID, bool fromReplicated);

    private:
        static void StartChildren(std::vector<GameObject*>& aChildComponents);

        static void PasteGameObject(GameObject*& aGameObject, rapidjson::Value& gameobject, rapidjson::Document::AllocatorType& anAllocator);
        static void PasteGameObjectSpecifiedIds(GameObject*& aGameObject, rapidjson::Value& gameobject, rapidjson::Document::AllocatorType& anAllocator,
            int aGameobjectID, const std::vector<unsigned>& aComponentsID, int currentIndex, bool fromReplicated);
    };

    ECL_API inline GameObject*& Instantiate(Assets::Prefab& aPrefab, GameObject* instagator = nullptr, bool Replicated = false)
    {
        GameObject* gameobject = InternalSpawnObjectClass::CreateObjectFromJsonString(aPrefab.dataPtr->data);
        gameobject->prefabAssetIDStr = aPrefab.GetAssetID().ToString();
        gameobject->IsPrefab = true;

        if (instagator)
            gameobject->transform->SetPosition(instagator->transform->GetPosition());
        
#ifdef ECLIPSED_NETWORKING
        if (Replicated)
            Replication::ReplicationManager::SendPrefabObject(gameobject, aPrefab);
#endif

        aPrefab.dataPtr->gameobject = gameobject;
        return aPrefab.dataPtr->gameobject;
    }

    ECL_API inline void Destroy(GameObject* aGameobject)
    {
        ComponentManager::Destroy(aGameobject->GetID());
    }

    ECL_API inline void Destroy(Component* component)
    {
        ComponentManager::DeleteComponent(component->gameObject->GetID(), component->myComponentComponentID, component->myInstanceComponentID);   
    }

    ECL_API inline GameObject* Find(const char* aName)
    {
        return ComponentManager::FindObjectByName(aName);
    }
}