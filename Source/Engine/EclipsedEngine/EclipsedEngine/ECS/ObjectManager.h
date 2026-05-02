#pragma once

#include <vector>

#include "rapidjson/document.h"

#include "EclipsedEngine/Replication/ReplicationManager.h"

#include "EclipsedEngine/Components/Transform2D.h"
#include "EntityEngine/GameObject.h"

#include "AssetEngine/AssetManager.h"

#include "CoreEngine/Macros/defines.h"



namespace Eclipse
{
    class ECLIPSED_API InternalSpawnObjectClass
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

    ECLIPSED_API inline GameObject*& Instantiate(Assets::Prefab& aPrefab, GameObject* instagator = nullptr, bool Replicated = false)
    {
        GameObject* gameobject = InternalSpawnObjectClass::CreateObjectFromJsonString(aPrefab.dataPtr->data);
        gameobject->prefabAssetIDStr = aPrefab.GetAssetID().ToString();
        gameobject->IsPrefab = true;

        if (instagator)
            gameobject->transform->SetPosition(instagator->transform->GetPosition());
        
        if (Replicated)
            Replication::ReplicationManager::SendPrefabObject(gameobject, aPrefab);
        
        aPrefab.dataPtr->gameobject = gameobject;
        return aPrefab.dataPtr->gameobject;
    }

    ECLIPSED_API inline void Destroy(Component* component)
    {
        ComponentManager::DeleteComponent(component->gameObject->GetID(), component->myComponentComponentID, component->myInstanceComponentID);   
    }

    ECLIPSED_API inline GameObject* Find(const char* aName)
    {
        return ComponentManager::FindObjectByName(aName);
    }
}