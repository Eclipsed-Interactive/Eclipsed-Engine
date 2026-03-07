#pragma once

// #include "EclipsedEngine/Components/Physics/RigidBody2D.h"
// #include "EclipsedEngine/Components/Physics/CapsuleCollider2D.h"

// #include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"
// #include "EclipsedEngine/Components/Rendering/SpriteSheetAnimator2D.h"
// #include "EclipsedEngine/Components/Player.h"
// #include "EclipsedEngine/Components/Transform2D.h"

#include "AssetEngine/Assets/Prefab.h"

#include "EclipsedEngine/Components/Component.h"

#include "EclipsedEngine/ECS/SpawnObject.h"
#include "Input/Input.h"

namespace Eclipse
{
    class PlayerSpawner : public Component
    {
    public:
        BASE_SELECTION(PlayerSpawner, 10);

        // void HasSpawnedHere_OnRep()
        // {

        // }

        void Update() override
        {
            if(Input::GetKeyDown(Keycode::L))
                Instantiate(playerPrefab, gameObject, true);
        }

        void Awake() override
        {
            

            // unsigned componentCount = 4;
            // NetMessage msg = NetMessage::BuildGameObjectMessage(0, MessageType::Msg_SendMultipleComponents, &componentCount, sizeof(unsigned), true);
            // MainSingleton::GetInstance<Client>().Send(msg);

            // GameObject* gameobject = ComponentManager::CreateGameObject();

            // Transform2D* transform = gameobject->AddComponent<Transform2D>(true);
            // transform->SetPosition(0, 1);
            // transform->SetScale(30, 30);

            // spriteRenderer = gameobject->AddComponent<SpriteRenderer2D>(true);
            // spriteRenderer->SetSprite(PlayerSprite);
            // gameobject->AddComponent<SpriteSheetAnimator2D>(true);

            // RigidBody2D* rb = gameobject->AddComponent<RigidBody2D>();
            // rb->SetRotationLocked(true);

            // CapsuleCollider2D* capsule = gameobject->AddComponent<CapsuleCollider2D>(true);

            // gameobject->AddComponent<Player>();






            //HasSpawnedHere = true;
        }

        // void Update()
        // {
        //     if (SetSpriteNextframe && !HasCreated)
        //     {
        //         spriteRenderer->SetSprite(PlayerSprite);
        //         HasCreated = true;
        //     }

        //     SetSpriteNextframe = true;

        //     if (Input::GetKeyDown(Keycode::J))
        //         spriteRenderer->SetSprite(OtherSprite);
        // }

        // bool SetSpriteNextframe = false;
        // bool HasCreated = false;

        // SpriteRenderer2D* spriteRenderer;

        // SERIALIZED_FIELD(Texture, PlayerSprite);
        // SERIALIZED_FIELD(Texture, OtherSprite);

        //REPLICATED_SERIALIZED_FIELD_DEFAULT(int, HasSpawnedHere, 0, PlayerSpawner);

        SERIALIZED_FIELD(Prefab, playerPrefab);

    };
}
