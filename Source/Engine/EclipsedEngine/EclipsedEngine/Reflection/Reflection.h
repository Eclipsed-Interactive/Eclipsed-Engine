#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "CoreEngine/Macros/defines.h"

namespace Eclipse 
{ 
	class Component;
}

namespace Eclipse::Reflection
{
	class AbstractSerializedVariable;
	class ECLIPSED_API ReflectionManager
	{
	private:
		using VariableList = std::vector<AbstractSerializedVariable*>;
		using CompToTypeList = std::unordered_map<Component*, VariableList>;

		static inline CompToTypeList registeredVariables;

	public:
		static void RegisterVariable(AbstractSerializedVariable* ptr);
		static void UnregisterVariable(AbstractSerializedVariable* ptr);

#ifdef ECLIPSED_EDITOR
		static void DrawInspector(Component* aComp, const char* name);
#endif

		static CompToTypeList& GetList();

		static void ClearList();
	};
}