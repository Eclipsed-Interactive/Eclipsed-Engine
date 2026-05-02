#pragma once

#include "AssetEngine/Assets/Prefab/PrefabAsset.h"

#include "EclipsedEngine/Components/Component.h"

#include "EclipsedEngine/ECS/ObjectManager.h"
#include "Input/Input.h"

namespace Eclipse
{
    class PlayerSpawner : public Component
    {
    public:
        BASE_SELECTION(PlayerSpawner, 10)

        void Start() override
        {
            if (Replication::ReplicationManager::ClickedHostButton)
            {
                Replication::ReplicationManager::Start(false);
            }
            else
            {
                Replication::ReplicationManager::Start(true);
            }

            Instantiate(soulSuckerPrefab, gameObject, true);
            Instantiate(soulForgePrefab, gameObject, true);
        }
        
        SERIALIZED_FIELD(Assets::Prefab, soulForgePrefab);
        SERIALIZED_FIELD(Assets::Prefab, soulSuckerPrefab);

    };
}
