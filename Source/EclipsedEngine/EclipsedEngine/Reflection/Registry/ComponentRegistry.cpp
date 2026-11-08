#include "ComponentRegistry.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace Eclipse
{
	std::unordered_map<std::string, RegisteredComp> ComponentRegistry::rttiNameToCompData;
	std::unordered_map<std::string, RegisteredComp> ComponentRegistry::compNameToCompData;

	void ComponentRegistry::RegisterComponent(const std::string& typeName, const std::string& rttiName, AddCompWithIdFunc addCompWithIdFunc, AddCompFunc addCompFunc, bool isGameComponent)
	{
		RegisteredComp comp{
			addCompWithIdFunc,
			addCompFunc,
			typeName,
			rttiName
		};

		rttiNameToCompData[rttiName] = comp;
		compNameToCompData[typeName] = comp;

		if (isGameComponent) gameComponents.push_back(typeName);
	}

	bool ComponentRegistry::IsRegistered(const std::string& name)
	{
		return compNameToCompData.find(name) != compNameToCompData.end() ||
			rttiNameToCompData.find(name) != rttiNameToCompData.end();
	}

	void ComponentRegistry::ClearRegisteredGameComponents()
	{
		for (const std::string& typeName : gameComponents)
		{
			RegisteredComp& comp = compNameToCompData[typeName];

			rttiNameToCompData.erase(comp.rttiName);
			compNameToCompData.erase(typeName);
		}
	}

	AddCompFunc ComponentRegistry::GetAddComponentByTypeName(const std::string& typeName)
	{
		if (IsRegistered(typeName))
			return compNameToCompData[typeName].addComp;

		return [](unsigned) { return nullptr; };
	}

	AddCompFunc ComponentRegistry::GetAddComponentByRttiTypeName(const std::string& rttiTypeName)
	{
		if (IsRegistered(rttiTypeName))
			return rttiNameToCompData[rttiTypeName].addComp;

		return [](unsigned) { return nullptr; };
	}

	AddCompWithIdFunc ComponentRegistry::GetAddComponentWithIdByTypeName(const std::string& typeName)
	{
		if (IsRegistered(typeName))
			return compNameToCompData[typeName].addCompWithId;

		return [](unsigned, unsigned) { return nullptr; };
	}

	AddCompWithIdFunc ComponentRegistry::GetAddComponentWithIdByRttiTypeName(const std::string& rttiTypeName)
	{
		if (IsRegistered(rttiTypeName))
			return rttiNameToCompData[rttiTypeName].addCompWithId;

		return [](unsigned, unsigned) { return nullptr; };
	}
	std::unordered_map<std::string, RegisteredComp>& ComponentRegistry::GetComponentRttiMap()
	{
		return rttiNameToCompData;
	}
	std::unordered_map<std::string, RegisteredComp>& ComponentRegistry::GetComponentTypeNameMap()
	{
		return compNameToCompData;
	}
} // namespace Eclipse