#pragma once

#include "AssetEngine/Assets/Prefab.h"

#include "EclipsedEngine/Components/Component.h"

#include "EclipsedEngine/ECS/SpawnObject.h"
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
                Replication::ReplicationManager::Start(false);
            else
                Replication::ReplicationManager::Start(true);

            Instantiate(playerPrefab, gameObject, true);
        }
        
        SERIALIZED_FIELD(Prefab, playerPrefab);

    };
}
