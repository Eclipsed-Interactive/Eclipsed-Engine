#pragma once

#ifdef ECLIPSED_NETWORKING
	#include "ReplicatedVariable.h"
	#include "ReplicationManager.h"

	// Use this as a standalone Macro with the serilized variable as arg
	//#define REPLICATE(Variable) Replication::ReplicationManager::RealReplicatedVariableList.at(myInstanceComponentID)[sprite.ReplicatedVariableIndex]->ReplicateThis(sprite.ReplicatedVariableIndex, false);
	//#define REPLICATEGARANTIED(Variable) Replication::ReplicationManager::RealReplicatedVariableList.at(myInstanceComponentID)[sprite.ReplicatedVariableIndex]->ReplicateThis(sprite.ReplicatedVariableIndex, true);

	//template<class T>
	//concept ConceptIsAsset = requires(T var) {var.IsAsset();};


	#define REPLICATEDVARIABLEDEFINE(Name, Data, DataCount, Auto, Type, ThisType)                                                        \
	Eclipse::Replication::ReplicatedVariable<ThisType> Repl##Name{Data, DataCount, this, Auto, &ThisType::Name##_OnRep}

	#define ONREPPEDFUNCTIONDEF(Name) void Name##_OnRep()

#else
	#define REPLICATEDVARIABLEDEFINE(Name, Data, DataCount, Auto, Type, ThisType)
	#define ONREPPEDFUNCTIONDEF(Name)
#endif


#define REPLICATED_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType)                     \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name);                                                                \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)

#define REPLICATED_SERIALIZED_FIELD(Type, Name, ThisType)                             \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f}; \
ONREPPEDFUNCTIONDEF(Name);                                                  \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)

#define REPLICATED_SERIALIZED_FIELD_DEFAULT_MIN_MAX(Type, Name, DefaultValue, Min, Max, ThisType)             \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f, DefaultValue, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name);                                                                          \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)

#define REPLICATED_SERIALIZED_FIELD_MIN_MAX(Type, Name, Min, Max, ThisType)                     \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name);                                                            \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)



#define REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(Type, Name, Step, DefaultValue, ThisType)           \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name);                                                                 \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)

#define REPLICATED_SERIALIZED_FIELD_STEP(Type, Name, Step, ThisType)                   \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step}; \
ONREPPEDFUNCTIONDEF(Name);                                                   \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)

#define REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT_MIN_MAX(Type, Name, Step, DefaultValue, Min, Max, ThisType)   \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name);                                                                           \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)

#define REPLICATED_SERIALIZED_FIELD_STEP_MIN_MAX(Type, Name, Step, Min, Max, ThisType)           \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name);                                                             \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)



#define REPLICATED_PRIVATE_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)

#define REPLICATED_PRIVATE_SERIALIZED_FIELD(Type, Name, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false};  \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), true, Type, ThisType)





#define MANUAL_REPLICATED_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f, DefaultValue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)

#define MANUAL_REPLICATED_SERIALIZED_FIELD(Type, Name, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_DEFAULT_MIN_MAX(Type, Name, DefaultValue, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f, DefaultValue, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_MIN_MAX(Type, Name, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 0.1f, Min, Max}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)



#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT(Type, Name, Step, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP(Type, Name, Step, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP_DEFAULT_MIN_MAX(Type, Name, Step, DefaultValue, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue, Min, Max, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)

#define MANUAL_REPLICATED_SERIALIZED_FIELD_STEP_MIN_MAX(Type, Name, Step, Min, Max, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, Min, Max, true}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)



#define MANUAL_REPLICATED_PRIVATE_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, 0.1f, DefaultV alue}; \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)

#define MANUAL_REPLICATED_PRIVATE_SERIALIZED_FIELD(Type, Name, ThisType) \
Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, 0.1f};  \
ONREPPEDFUNCTIONDEF(Name); \
REPLICATEDVARIABLEDEFINE(Name, Name.GetData(), sizeof(Type), false, Type, ThisType)