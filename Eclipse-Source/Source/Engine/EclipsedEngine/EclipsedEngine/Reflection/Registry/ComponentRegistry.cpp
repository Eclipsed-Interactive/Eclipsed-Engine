#include "ComponentRegistry.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eclipse
{
	// Define static member variables (they live only in the DLL)
	std::unordered_map<std::string, std::function<Component* (unsigned, unsigned)>> ComponentRegistry::addComponentMap;
	std::unordered_map<std::string, std::function<Component* (unsigned)>> ComponentRegistry::inspectorAddComponentMap;

	bool ComponentRegistry::IsRegisteredInspector(const std::string& typeName)
	{
		return inspectorAddComponentMap.find(typeName) != inspectorAddComponentMap.end();
	}

	bool ComponentRegistry::IsRegisteredScene(const std::string& typeName)
	{
		return addComponentMap.find(typeName) != addComponentMap.end();
	}

	void ComponentRegistry::ClearRegisteredGameComponents()
	{
		for (const std::string& typeName : gameComponents)
		{
			addComponentMap.erase(typeName);
			inspectorAddComponentMap.erase(typeName);
		}
	}

	void ComponentRegistry::Register(const std::string& typeName,
		std::function<Component* (unsigned, unsigned)> addComponentMethod, bool isGame)
	{
		auto& map = GetAddComponentMap();
		map[typeName] = addComponentMethod;

		if (isGame) gameComponents.push_back(typeName);
	}

	std::function<Component* (unsigned, unsigned)> ComponentRegistry::GetAddComponent(const std::string& typeName)
	{
		auto& map = GetAddComponentMap();
		auto it = map.find(typeName);
		if (it != map.end())
		{
			return it->second;
		}
		return [](unsigned, unsigned) { return nullptr; };
	}

	std::unordered_map<std::string, std::function<Component* (unsigned, unsigned)>>&
		ComponentRegistry::GetAddComponentMap()
	{
		return addComponentMap;
	}

	void ComponentRegistry::RegisterInspector(const std::string& typeName,
		std::function<Component* (unsigned)> addComponentMethod, bool isGame)
	{
		auto& map = GetInspectorAddComponentMap();
		map[typeName] = addComponentMethod;

		if (isGame) gameComponents.push_back(typeName);
	}

	std::function<Component* (unsigned)> ComponentRegistry::GetInspectorAddComponent(const std::string& typeName)
	{
		auto& map = GetInspectorAddComponentMap();
		auto it = map.find(typeName);
		if (it != map.end())
		{
			return it->second;
		}
		return [](unsigned) { return nullptr; };
	}

	std::unordered_map<std::string, std::function<Component* (unsigned)>>&
		ComponentRegistry::GetInspectorAddComponentMap()
	{
		return inspectorAddComponentMap;
	}
} // namespace Eclipse