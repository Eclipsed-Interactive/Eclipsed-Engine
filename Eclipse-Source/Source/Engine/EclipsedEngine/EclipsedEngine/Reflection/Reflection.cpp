#include "Reflection.h"

#include "ImGui/imgui.h"

#include "ImGui/imgui.h"
#include "SerializedVariable.h"

namespace Eclipse::Reflection
{
	void ReflectionManager::RegisterVariable(AbstractSerializedVariable* ptr)
	{
		VariableList& list = registeredVariables[ptr->pComponent];
		list.push_back(ptr);
	}

	void ReflectionManager::UnregisterVariable(AbstractSerializedVariable* ptr)
	{
		if (registeredVariables.find(ptr->pComponent) == registeredVariables.end()) return;
		VariableList& list = registeredVariables[ptr->pComponent];

		if (list.empty()) return;

		auto it = std::find(list.begin(), list.end(), ptr);
		if (it != list.end()) {
			list.erase(it);
		}


		if (list.empty()) {
			registeredVariables.erase(ptr->pComponent);
		}
	}

#ifdef ECLIPSED_EDITOR

	void ReflectionManager::DrawInspector(Component* aComp, const char* name)
	{
		if (registeredVariables.find(aComp) == registeredVariables.end()) return;
		VariableList& list = registeredVariables.at(aComp);

		for (auto& element : list)
		{
			if (!element->canDrawInspector) continue;
				element->DrawTest();

			//element->DrawInspector();
		}
	}
#endif

	ReflectionManager::CompToTypeList& ReflectionManager::GetList()
	{
		return registeredVariables;
	}

	void ReflectionManager::ClearList()
	{
		registeredVariables.clear();
	}
}