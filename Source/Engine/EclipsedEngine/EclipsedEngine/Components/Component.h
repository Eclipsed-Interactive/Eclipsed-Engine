//#ifndef _ECL_ENG_COMPONENT
//#define _ECL_ENG_COMPONENT

#include "EntityEngine/BaseComponent.h"
#include "EclipsedEngine/TypeIDGenerator.h"

#include "EclipsedEngine/Reflection/Reflection_Macros.h"
#include "EclipsedEngine/Replication/Replication_Macros.h"



#define BASE_OBJECT_IMPL(TYPE)																\
static inline unsigned typeID = Eclipse::TypeRegistry::GetTypeID<TYPE>();					\
static unsigned GetTypeID() { return typeID; }


//#include "CoreEngine/Reflection/Registry/ComponentRegistry.h"

#include "EntityEngine/ComponentManager.h"

#pragma warning (push)
#pragma warning (disable:4005)
#define BASE_MACRO_NAME COMPONENT_BASE
#pragma warning(pop)

#include "CoreEngine/Macros/MacroOverloadSelector.h"

#define COMPONENT_FRIEND_CLASS         \
friend class Eclipse::Editor::InspectorWindow;  \
friend class ComponentManager;         \
friend class SceneLoader;           

#define COMPONENT_BASE_1(type)												\
COMPONENT_FRIEND_CLASS														\
BASE_OBJECT_IMPL(type)																				\
public:																		\
inline type() = default;													\
virtual ~type() = default;													\
private:


#define REGISTER_COMPONENT_CALLBACK(type) [](unsigned gameObjId, unsigned compID){ return Eclipse::ComponentManager::AddComponentWithID<type>(gameObjId, compID); }
#define REGISTER_COMPONENT_CALLBACK_NORMAL(type) [](unsigned gameObjId){ return Eclipse::ComponentManager::AddComponent<type>(gameObjId); }



#define COMPONENT_BASE_2(type, updatePriority)														\
COMPONENT_FRIEND_CLASS																				\
BASE_OBJECT_IMPL(type)																				\
public:																								\
virtual unsigned GetUpdatePriority() const override {return updatePriority;}						\
inline type() = default;																			\
virtual ~type() = default;																			\
virtual const char* GetComponentName() override { return #type; }							\
protected:




#define COMPONENT_BASE_3(type, derivedType, updatePriority)				\
COMPONENT_FRIEND_CLASS														\
BASE_OBJECT_IMPL(type)																				\
public:																		\
    inline type() = default;										\
virtual ~type() = default;													\
protected:																	\
virtual const char* GetComponentName() override { return #type; }	\
private:


#define FORCELINK(Class)												\
	namespace {															\
		static auto& forceLink = Eclipse::Class::_register;				\
	}


#define COMP_REG(TYPE)																						\
Eclipse::ComponentRegistry::Register(#TYPE, REGISTER_COMPONENT_CALLBACK(TYPE));								\
Eclipse::ComponentRegistry::RegisterInspector(#TYPE, REGISTER_COMPONENT_CALLBACK_NORMAL(TYPE));


#define COMPONENT_REGISTRATION(TYPE)																	\
struct AutoRegister_##TYPE {																				\
	AutoRegister_##TYPE() {																					\
		using namespace Eclipse;																		\
		ComponentRegistry::Register(#TYPE, REGISTER_COMPONENT_CALLBACK(TYPE));							\
		ComponentRegistry::RegisterInspector(#TYPE, REGISTER_COMPONENT_CALLBACK_NORMAL(TYPE));			\
	}																									\
};																										\
static AutoRegister_##TYPE _auto_registrator_##TYPE = {};


//#endif