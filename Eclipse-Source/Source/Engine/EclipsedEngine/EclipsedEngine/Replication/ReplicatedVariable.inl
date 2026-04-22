#include "ReplicatedVariable.h"
#include "ReplicationManager.h"

#include "Eclipsedengine/Reflection/Reflection.h"
#include "CoreEngine/MainSingleton.h"

#include "NetworkEngine/Server/Server.h"
#include "NetworkEngine/Client/Client.h"

#include "EclipsedEngine/Components/Component.h"
#include "EntityEngine/GameObject.h"

#include "AssetEngine/Data/AssetData.h"
//#include "AssetEngine/Models/AssetDatas/Handles/AssetHandle.h"

namespace Eclipse::Replication
{
    template<typename T>
    ReplicatedVariable<T>::ReplicatedVariable(std::string aName, Component* aComponent, bool anAutomatic,
        unsigned ID, int aReplicationIndex, void(T::* OnRepFunctionPtr)(), bool aIsAsset) : OnRepFunction(OnRepFunctionPtr)
    {
        ConnectedComponent = aComponent;

        bool variableExist = false;

        auto& reflectionList = Reflection::ReflectionManager::GetList();
        auto& variableList = reflectionList.at(aComponent);

        for (auto& variable : variableList)
        {
            if (variable->GetName() == aName)
            {
                variable->ResolveTypeInfo();
                
                ManualVariableSending = !anAutomatic;
                variableExist = true;

                IsAsset = aIsAsset;

                myReflectVariable = variable;

                if (aIsAsset)
                    dataAmount = 32;
                else
                    dataAmount = variable->GetSizeInBytes();

                variable->ReplicatedVariableIndex = aReplicationIndex++;

                break;
            }
        }

        assert(variableExist);

        ReplicationManager::EmplaceReplicatedVariable(0, this);
    }
}