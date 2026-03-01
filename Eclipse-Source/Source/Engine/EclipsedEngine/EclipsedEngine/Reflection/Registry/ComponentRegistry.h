#pragma once

#include <functional>
#include <unordered_map>
#include <string>

#include "CoreEngine/Macros/defines.h"

namespace Eclipse
{
    class Component;

    class ComponentRegistry final
    {
    public:
        ComponentRegistry() = delete;
        ~ComponentRegistry() = delete;

    private:
        using AddComponentMap = std::unordered_map<std::string, std::function<Component* (unsigned, unsigned)>>;
        using InspectorAddComponentMap = std::unordered_map<std::string, std::function<Component* (unsigned)>>;

        static ECLIPSED_API AddComponentMap addComponentMap;
        static ECLIPSED_API InspectorAddComponentMap inspectorAddComponentMap;

    public:
        static bool IsRegisteredInspector(const std::string& typeName);
        static bool IsRegisteredScene(const std::string& typeName);


        static void ClearRegisteredGameComponents();

        static void Register(const std::string& typeName, std::function<Component* (unsigned, unsigned)> addComponentMethod, bool isGame = false);
        static std::function<Component* (unsigned, unsigned)> GetAddComponent(const std::string& typeName);
        static std::unordered_map<std::string, std::function<Component* (unsigned, unsigned)>>& GetAddComponentMap();

        static void RegisterInspector(const std::string& typeName, std::function<Component* (unsigned)> addComponentMethod, bool isGame = false);
        static std::function<Component* (unsigned)> GetInspectorAddComponent(const std::string& typeName);
        static std::unordered_map<std::string, std::function<Component* (unsigned)>>& GetInspectorAddComponentMap();

        static inline std::vector<std::string> gameComponents;
    };
} // namespace Eclipse
