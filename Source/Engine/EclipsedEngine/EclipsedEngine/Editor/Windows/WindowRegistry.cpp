#include "WindowRegistry.h"

#include "EclipsedEngine/Editor/Windows/WindowTypes/Window.h"

namespace Eclipse::Editor
{
	AbstractWindow* WindowRegistry::GetWindow(const std::string& name)
	{
		return registeredWindows[name];
	}

	const std::unordered_map<std::string, AbstractWindow*>& WindowRegistry::GetWindows()
	{
		return WindowRegistry::registeredWindows;
	}
}