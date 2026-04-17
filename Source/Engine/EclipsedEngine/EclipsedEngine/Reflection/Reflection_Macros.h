#ifndef REFLECTION_MACROS_H
#define REFLECTION_MACROS_H

#include "CoreEngine/SerializedEnum.hpp"
#include "Reflection.h"
#include "SerializedVariable.h"

//#ifdef ECLIPSED_EDITOR

#define SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, DefaultValue}
#define SERIALIZED_FIELD(Type, Name) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f}

#define SERIALIZED_FIELD_DEFAULT_MIN_MAX(Type, Name, DefaultValue, Min, Max) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, DefaultValue, Min, Max}
#define SERIALIZED_FIELD_MIN_MAX(Type, Name, Min, Max) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, 1.f, Min, Max}

#define PRIVATE_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, 1.f, DefaultValue}
#define PRIVATE_SERIALIZED_FIELD(Type, Name) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, false, 1.f}



#define SERIALIZED_FIELD_STEP_DEFAULT(Type, Name, Step, DefaultValue) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue}
#define SERIALIZED_FIELD_STEP(Type, Name, Step) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step}

#define SERIALIZED_FIELD_STEP_DEFAULT_MIN_MAX(Type, Name, Step, DefaultValue, Min, Max) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, DefaultValue, Min, Max}
#define SERIALIZED_FIELD_STEP_MIN_MAX(Type, Name, Step, Min, Max) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, true, Step, Min, Max}

//#elif _GAME
//
//#define SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, DefaultValue}
//#define SERIALIZED_FIELD(Type, Name) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this}
//
//#define SERIALIZED_FIELD_DEFAULT_MIN_MAX(Type, Name, DefaultValue, Min, Max) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this, DefaultValue}
//#define SERIALIZED_FIELD_MIN_MAX(Type, Name, Min, Max) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this}
//
//#define PRIVATE_SERIALIZED_FIELD_DEFAULT(Type, Name, DefaultValue) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this}
//#define PRIVATE_SERIALIZED_FIELD(Type, Name) Eclipse::Reflection::SerializedVariable<Type> Name{#Name, this}
//
#endif


//#endif