#include "SelectionContext.h"

namespace Eclipse::Editor
{
	void SelectionContext::SetActiveContext(InspectableTarget target)
	{
		Get().activeTarget = target;
	}
	InspectableTarget& SelectionContext::GetCurrentData()
	{
		return Get().activeTarget;
	}
}