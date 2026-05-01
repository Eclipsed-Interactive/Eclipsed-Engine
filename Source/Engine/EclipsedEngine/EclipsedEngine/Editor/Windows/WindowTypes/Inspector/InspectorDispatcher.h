#pragma once

#include "InspectableTarget.h"

namespace Eclipse::Editor
{
	class InspectorDispatcher
	{
	public:
		static void Draw(const InspectableTarget& target);
	};
}