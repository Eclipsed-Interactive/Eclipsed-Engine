#include "Window.h"

#include "CoreEngine/Math/Random.h"

Eclipse::Editor::AbstractWindow::AbstractWindow(const std::string& name, const int& aId)
	: instanceID(aId == -1 ? Random::Rand<int>() : aId), windowName(name)
{
}
