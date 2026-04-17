#pragma once

#include <string>

namespace Eclipse
{
    namespace Reflection
    {
        class AbstractSerializedVariable;
    }

    class Component;
    namespace Replication
    {
        class BaseReplicatedVariable
        {
        public:
            bool IsAsset;
            Reflection::AbstractSerializedVariable* myReflectVariable;
            bool ManualVariableSending;
            int dataAmount;

            void ReplicateThis(unsigned aID, bool aIsGarantied = false, bool aAlwaysSend = false);

            Component* ConnectedComponent;
        };

        template<typename T>
        class ReplicatedVariable : public BaseReplicatedVariable
        {
        public:
            inline ReplicatedVariable(std::string aName, Component* aComponent, bool anAutomatic, 
                unsigned ID, int aReplicationIndex, void(T::* OnRepFunctionPtr)(), bool aIsAsset);

        public:
            void(T::* OnRepFunction)();
        };
    }

}

#include "ReplicatedVariable.inl"