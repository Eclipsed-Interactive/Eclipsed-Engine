#pragma once

#ifdef ECLIPSED_NETWORKING

#include "Networking/Shared/Message.h"

#include <vector>
#include "Core/GUID.h"

#include "EclipsedEngine.Core.hpp"

namespace Eclipse
{
    class Component;
}

namespace Eclipse::Replication
{
    class ECL_API ReplicationHelper
    {
    public:
        class ClientHelp
        {
        public:
            static void StartReplicatedComponents();

            static void RecieveAmountOfComponents(const NetMessage& message);
            static void RecieveAddComponentMessage(const NetMessage& message);
            static void RecieveCreateObjectMessage(const NetMessage& message);
            static void RecieveDeleteObjectMessage(const NetMessage& message);
            static void RecieveInstantiatePrefabMessage(const NetMessage& message);
            static void RecieveVariableMessage(const NetMessage& message);
            static void RecieveRequestVariablesMessage(const NetMessage& message);

            static void HandleRecieve(const NetMessage& message);

            static inline std::vector<Component*> ComponentsToStartOnDemand;

            static inline unsigned myComponentsToRecieved = 9999;

            // Placing here cause other is bad.
            static void RefreshAsset(Reflection::AbstractSerializedVariable* aVariable, GUID aAssetID);
        };


    public:
        class ServerHelp
        {
        public:
            static void RequestVariablesFromClient();
            static void SendComponentsScene();
            static void HandleRequestedScene();

            static void HandleRecieve(const NetMessage& aMessage);
        };
    };
}

#endif