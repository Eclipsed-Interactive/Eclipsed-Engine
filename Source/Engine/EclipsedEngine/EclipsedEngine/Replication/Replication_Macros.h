#pragma once

#include "EclipsedEngine/Reflection/SerializedVariable.h"
#include "ReplicatedVariable.h"
#include "ReplicationManager.h"

// Use this as a standalone Macro with the serilized variable as arg
#define REPLICATE(Variable) Replication::ReplicationManager::RealReplicatedVariableList.at(myInstanceComponentID)[sprite.ReplicatedVariableIndex]->ReplicateThis(sprite.ReplicatedVariableIndex, false);
#define REPLICATEGARANTIED(Variable) Replication::ReplicationManager::RealReplicatedVariableList.at(myInstanceComponentID)[sprite.ReplicatedVariableIndex]->ReplicateThis(sprite.ReplicatedVariableIndex, true);

template<class T>
concept ConceptIsAsset = requires(T var) {var.IsAsset();};

#define REPLICATEHELPERMACRONOTUSE(Name, Auto, ReplicatedIndex, ThisType, Type)                                                                                               \
Eclipse::Replication::ReplicatedVariable<ThisType> Repl##Name{#Name, this, Auto, ReplicatedIndex, ReplicationIndex, &ThisType::Name##_OnRep, ConceptIsAsset<Type>}

#define ONREPPEDFUNCTIONDEF(Name) void Name##_OnRep()



#define REPLICATED_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)

#define REPLICATED_SERIALIZED_FIELD(Type, Name, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)

#define REPLICATED_SERIALIZED_FIELD_DEFAULT_MIN_MAX(Type, Name, DefaultValue, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, DefaultValue, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)

#define REPLICATED_SERIALIZED_FIELD_MIN_MAX(Type, Name, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)



#define REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(Type, Name, Step, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)

#define REPLICATED_SERIALIZED_FIELD_STEP(Type, Name, Step, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)

#define REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT_MIN_MAX(Type, Name, Step, DefaultValue, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)

#define REPLICATED_SERIALIZED_FIELD_STEP_MIN_MAX(Type, Name, Step, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)



#define REPLICATED_PRIVATE_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)

#define REPLICATED_PRIVATE_SERIALIZED_FIELD(Type, Name, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false};  \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, true, Name.ReplicatedVariableIndex, ThisType, Type)





#define MANUAL_REPLICATED_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)

#define MANUAL_REPLICATED_SERIALIZED_FIELD(Type, Name, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_DEFAULT_MIN_MAX(Type, Name, DefaultValue, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, DefaultValue, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_MIN_MAX(Type, Name, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)



#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(Type, Name, Step, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP(Type, Name, Step, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT_MIN_MAX(Type, Name, Step, DefaultValue, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue, Min, Max, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP_MIN_MAX(Type, Name, Step, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, Min, Max, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)



#define MANUAL_REPLICATED_PRIVATE_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, 1.f, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)

#define MANUAL_REPLICATED_PRIVATE_SERIALIZED_FIELD(Type, Name, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, 1.f};  \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEHELPERMACRONOTUSE(Name, false, Name.ReplicatedVariableIndex, ThisType, Type)