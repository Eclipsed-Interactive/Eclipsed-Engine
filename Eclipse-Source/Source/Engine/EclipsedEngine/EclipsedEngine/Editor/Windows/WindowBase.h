#ifndef _WINDOW_BASE_H
#define _WINDOW_BASE_H

#include "WindowTypes/Window.h"	
#include "EclipsedEngine/Editor/Windows/WindowRegistry.h"

/**
* @brief BASE_SELECTION will do different things depending on the amount of arguments passed.
* @param BASE_SELECTION(Type Name)
* @param BASE_SELECTION(Type Name Category) ### Category can be "1/2/3" must be split with '/'.
*/
#define HELP

#pragma warning (push)
#pragma warning (disable:4005)

#define BASE_MACRO_NAME EDITOR_WINDOW_BASE

#pragma warning(pop)

#include "CoreEngine/Macros/MacroOverloadSelector.h"	

#define EDITOR_WINDOW_BASE_1(type)                                           \
public:                                                                             \
    const size_t typeID = std::hash<std::string>{}(#type);                          \
    inline type(const int& aId = -1) : AbstractWindow(#type, aId) {}                \
    virtual AbstractWindow* GetNewWindow(const int& aId = -1) override {           \
        return new type(aId);                                                      \
    }                                                                              \
private:

#ifdef ECLIPSED_EDITOR
#define EDITOR_WINDOW_BASE_2(type, name)                                           \
public:                                                                             \
    const size_t typeID = std::hash<std::string>{}(#type);                          \
    inline type(const int& aId = -1) : AbstractWindow(name, aId) {}                \
    virtual AbstractWindow* GetNewWindow(const int& aId = -1) override {           \
        return new type(aId);                                                      \
    }                                                                              \
private:                                                                            \
    struct AutoRegister {                                                           \
        AutoRegister() {                                                            \
            WindowRegistry::RegisterWindow<type>(name); \
        }                                                                          \
    };                                                                             \
    static inline AutoRegister _register = {};
#else
#define EDITOR_WINDOW_BASE_2(type, name) EDITOR_WINDOW_BASE_1(type)
#endif


#define EDITOR_WINDOW_BASE_3(type, name, category)           \
public:                                                       \
    const char* GetCategoryName() override { return category; } \
    EDITOR_WINDOW_BASE_2(type, name)

#endif