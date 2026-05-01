#pragma once

#include <filesystem>
#include "InspectableTarget.h"

namespace Eclipse::Editor {
	class IInspector {
	public:
		IInspector() = default;
		virtual bool CanInspect(const InspectableTarget& target) { return true; }
		virtual void Draw(const InspectableTarget& target) = 0;
	};
}