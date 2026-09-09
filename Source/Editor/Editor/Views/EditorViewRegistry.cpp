#include "EditorViewRegistry.h"

#include "Editor/Views/IEditorView.h"
#include "EditorViewRegistry.h"

namespace Eclipse::Editor
{
	const EditorViewType* EditorViewRegistry::GetViewType(std::string_view viewName)
	{
		auto it = registeredViewsTypes.find(std::string(viewName));
		if (it == registeredViewsTypes.end()) return nullptr;

		return it->second;
	}

	std::vector<EditorViewType*> EditorViewRegistry::GetViewTypes()
	{
		std::vector<EditorViewType*> result;
		for (const auto [name, ptr] : registeredViewsTypes)
			result.push_back(ptr);

		return result;
	}
}