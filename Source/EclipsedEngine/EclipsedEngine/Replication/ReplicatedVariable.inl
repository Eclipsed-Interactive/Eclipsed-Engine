#ifdef ECLIPSED_NETWORKING

#include "ReplicatedVariable.h"
#include "ReplicationManager.h"

#include "EclipsedEngine/Components/Component.h"

namespace Eclipse::Replication
{
    template<typename T>
    ReplicatedVariable<T>::ReplicatedVariable(void* aData, int aDataCount, Component* aComponent, bool anAutomatic, void(T::* aOnRepFunction)()) :
        BaseReplicatedVariable(aData, aDataCount, aComponent, anAutomatic), 
        OnRepFunction(aOnRepFunction)
    {
        ReplicationManager::EmplaceReplicatedVariable(OnComponent->myInstanceComponentID, IterationID, this);
    }
}

#endif